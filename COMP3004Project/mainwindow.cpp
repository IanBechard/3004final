#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(batteryTimer, SIGNAL(timeout()), this, SLOT(updateCaption())); //initialize battery timer
    connect(programTimer, SIGNAL(timeout()), this, SLOT(updateProgramTimer())); //initialize program/frequency timers
    power_off();


}


/*  power_on();
 *  initializes device, bringing user to home screen and enabling battery depletion
 */
void MainWindow::power_on(){
    menu = "";
    batteryBeep();
    power.setPower(0);

    inTreatment=false;
    powerOn=true;
    toggleButtons();
    ui->timeLabel->clear();
    ui->count->clear();
    ui->powerLabel->clear();
    ui->power->clear();
    ui->listWidget->setStyleSheet("""QListWidget{background: white;}""");
    ui->trodeButton->setStyleSheet("background-color: red");
    setMainMenu();
    ui->menuLabel->setText("Main Menu");
    on_downButton_clicked();
    power.setPower(0);
    electrodesConnected = false;
    ui->batteryTag->setText(QString::number(round(b.getPercentage())));
    batteryTimer->start(5000);//"tick" rate for battery
}


/*  power_off();
 *  resets device, disables buttons, interface and battery depletion
 */
void MainWindow::power_off(){
    powerOn=false;
    toggleButtons();

    ui->batteryTag->clear();
    ui->listWidget->clear();
    ui->menuLabel->clear();
    ui->trodeButton->setStyleSheet(ui->backButton->styleSheet());
    electrodesConnected=false;

    ui->listWidget->setStyleSheet("""QListWidget{background: black;}""");
    batteryTimer->stop();
    programTimer->stop();
}
/*  batteryBeep();
 *  called when low battery warning is needed
 */
void MainWindow::batteryBeep(){
        ui->batteryTag->setText("LOW BATTERY");


}


/*  toggleButtons();
 *  for toggling button functionality on/off for power cycling
 */
void MainWindow::toggleButtons(){
    if (powerOn){
    ui->backButton->setEnabled(true);
    ui->trodeButton->setEnabled(true);
    ui->upButton->setEnabled(true);
    ui->downButton->setEnabled(true);
    ui->leftButton->setEnabled(true);
    ui->rightButton->setEnabled(true);
    ui->selectButton->setEnabled(true);
    ui->homeButton->setEnabled(true);
    }
    else{
        ui->backButton->setEnabled(false);
        ui->trodeButton->setEnabled(false);
        ui->upButton->setEnabled(false);
        ui->downButton->setEnabled(false);
        ui->leftButton->setEnabled(false);
        ui->rightButton->setEnabled(false);
        ui->selectButton->setEnabled(false);
        ui->homeButton->setEnabled(false);
    }
}

/*  updateCaption();
 *  runs every x seconds, depleting the battery based on power level
 */
void MainWindow::updateCaption(){
    b.degenerate();
    ui->batteryTag->setText(QString::number(round(b.getPercentage())));
    if (b.getPercentage() <= 0){
        power_off();
    }
    if (b.getPercentage() < 10 && lowBatteryTriggered == false){
        batteryBeep();
        lowBatteryTriggered = true;
    }
}

/*  updateProgramTimer();
 *  runs every second when in a treatment, depending on treatment option chosen by user
 */
void MainWindow::updateProgramTimer(){

    if(menu == "Frequencies"){
         if (electrodesConnected == true){       //if electrodes are on...
            updateMenuCounter++;
            programTime->addSecs(1);

        }
        timerText = timerFormat(updateMenuCounter);
    }
    else{//menu == "Programs"
        if (updateMenuCounter >= progsMenu.getPrograms()[menuType].getTime()){                                            //if the treatment has reached its end
            programTimer->stop();
            timerText.clear();
            backMenuHandler("");
            return;
        }
        else if ((electrodesConnected) == (true && updateMenuCounter <= progsMenu.getPrograms()[menuType].getTime())){       //if electrodes are on...
            updateMenuCounter++;
            programTime->addSecs(-1);
        }

        timerText = timerFormat(progsMenu.getPrograms()[menuType].getTime()-updateMenuCounter);
    }

    ui->count->setText(timerText);
}

/*  destructor
 *  closes interface
 */
MainWindow::~MainWindow()
{
    delete ui;
}


/*  down handler
 *  validates selection in a menu
 */
void MainWindow::on_downButton_clicked()
{
    ui->listWidget->setCurrentRow(validSelection(ui->listWidget->currentRow() + 1, ui->listWidget->count()));
}


/*  electrode button handler
 *  enables/disables electrodes being connected
 */
void MainWindow::on_trodeButton_clicked()
{
    if (electrodesConnected){
        electrodesConnected = false;
        b.setDegen(1);
       ui->trodeButton->setStyleSheet("background-color: red");
    }
    else{
        electrodesConnected = true;
        if (power.getPower() == 0){
            b.setDegen(1);
        }
        else{
        b.setDegen(power.getPower());
        }
        ui->trodeButton->setStyleSheet("background-color: green");

    }
}


/*  home button handler
 *  brings user back to the home/main screen and resets all data
 */
void MainWindow::on_homeButton_clicked(){
    menu = "";
    progOrFreq = 0;
    electrodesConnected = false;
    ui->trodeButton->setStyleSheet("background-color: red");
    programTimer->stop();
    power.setPower(0);
    b.setDegen(1);
    inTreatment=false;
    ui->timeLabel->clear();
    ui->count->clear();
    ui->powerLabel->clear();
    ui->power->clear();
    setMainMenu();
    ui->menuLabel->setText("Main Menu");//set header to main menu
    on_downButton_clicked();
}


/*  up button handler
 *  validates selection in a menu
 */
void MainWindow::on_upButton_clicked()
{
    ui->listWidget->setCurrentRow(validSelection(ui->listWidget->currentRow() - 1, ui->listWidget->count()));
}


/*  select button handler
 *  goes deeper in the menu based upon selection, or does nothing if in a treatment
 */
void MainWindow::on_selectButton_clicked()
{
    if (progOrFreq >= 1) {
        // DO NOTHING
    }
    else {
     selectMenuHandler(ui->listWidget->currentItem()->text());
    }
}

/*  back button handler
 *  calls function that brings user back one menu based on current state
 */
void MainWindow::on_backButton_clicked()
{
    if (progOrFreq >= 1){
        backMenuHandler("");
    }
    else{
    backMenuHandler(ui->listWidget->currentItem()->text());
    }
}

/*  updateList()
 *  updates the menu based upon user selection
 */
void MainWindow::updateList(std::vector<QString> l)
{
    ui->listWidget->clear();

    for(auto item: l){
        ui->listWidget->addItem(item);
    }

}

//for verifying that up/down buttons will never leave the menu
int MainWindow::validSelection(int x, int count)
{
    if (x < 0){
        return x+1;
    }
    else if (x > count-1) {
        return x-1;
    }
    return x;

}

/*  selectMenuHandler()
 *  brings user into the treatment menu
 */
void MainWindow::selectMenuHandler(QString s)
{
    if (s == "Program"){
        updateList(progsMenu.getProgramsNames());
        ui->menuLabel->setText("Program");
        //setheader to "Programs"
        on_downButton_clicked();
    }
    else if (s == "Frequency"){
        updateList(freqMenu.getFrequenciesNames());
        ui->menuLabel->setText("Frequency");
        //setheader to "Frequencies"
        on_downButton_clicked();
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    else if (s == "Allergy"){
        progOrFreq = 1;
        inTreatment = true;
        updateMenuCounter = 0;
        menuType = 0;
        menu = "Programs";
        programTime->setHMS(0,0,(progsMenu.getPrograms()[menuType].getTime()));
        ui->listWidget->clear();
        // Initialize labels and QTimer
        ui->timeLabel->setText("Time remaining: ");
        ui->powerLabel->setText("Power: ");
        ui->power->setText("0");
        ui->count->setText(programTime->toString("mm:ss"));
        programTimer->start(1000);

    }
    else if (s == "Pain"){
        progOrFreq = 1;
        inTreatment = true;
        updateMenuCounter = 0;
        menuType = 1;
        menu = "Programs";
        programTime->setHMS(0,0,(progsMenu.getPrograms()[menuType].getTime()));
        ui->listWidget->clear();
        ui->timeLabel->setText("Time remaining: ");
        ui->powerLabel->setText("Power: ");
        ui->power->setText("0");
        ui->count->setText(programTime->toString("mm:ss"));
        programTimer->start(1000);

    }
    else if (s == "Bloating"){
        progOrFreq = 1;
        inTreatment = true;
        updateMenuCounter = 0;
        menuType = 2;
        menu = "Programs";
        programTime->setHMS(0,0,(progsMenu.getPrograms()[menuType].getTime()));
        ui->listWidget->clear();
        ui->timeLabel->setText("Time remaining: ");
        ui->powerLabel->setText("Power: ");
        ui->power->setText("0");
        ui->count->setText(programTime->toString("mm:ss"));
        programTimer->start(1000);

    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    else if (s == "1.0-9.9 Hz"){
        startFrequency();

    }
    else if (s == "10 Hz"){
        startFrequency();

    }
    else if (s == "20 Hz"){
        startFrequency();

    }
}

/*  startFrequency()
 *  initializes frequency start
 */
void MainWindow::startFrequency(){
    progOrFreq = 2;
    inTreatment = true;
    updateMenuCounter = 0;
    menu = "Frequencies";
    programTime->setHMS(0,0,0);
    ui->listWidget->clear();
    ui->timeLabel->setText("Time: ");
    ui->powerLabel->setText("Power: ");
    ui->power->setText("0");
    ui->count->setText(programTime->toString("mm:ss"));
    programTimer->start(1000);
}


/*  backMenuHandler
 *  brings user back 1 menu based on their current state
 */
void MainWindow::backMenuHandler(QString s)
{
    std::vector<QString> programs = progsMenu.getProgramsNames();
    std::vector<QString> frequencies  = freqMenu.getFrequenciesNames();
    if (progOrFreq == 1){
        treatmentBack(programs);
        ui->menuLabel->setText("Program");
    }
    else if (progOrFreq == 2){
        treatmentBack(frequencies);
        ui->menuLabel->setText("Frequency");
    }
    progOrFreq = 0;
    if (count(programs.begin(), programs.end(),  s) > 0 || count(frequencies.begin(), frequencies.end(),  s) > 0 ) // || count(settingsS.begin(), settingsS.end(),  s) > 0
    {
        setMainMenu();
        ui->menuLabel->setText("Main Menu");
        //set header to main menu
        on_downButton_clicked();
    }

}

/*  treatmentBack()
 *  called when a user is in the treatment menu and presses back
 */
void MainWindow::treatmentBack(std::vector<QString> t){
    electrodesConnected = false;
    ui->trodeButton->setStyleSheet("background-color: red");
    menu = "";
    programTimer->stop();
    power.setPower(0);
    inTreatment=false;
    ui->timeLabel->clear();
    ui->count->clear();
    ui->powerLabel->clear();
    ui->power->clear();
    updateList(t);
    //setheader to "Programs"
    on_downButton_clicked();
}
/*  setMainMenu()
 *  puts user to main menu
 */
void MainWindow::setMainMenu()
{
    std::vector<QString> mainmenu{"Program", "Frequency", "MED", "Screening", "Children", "Settings"};
    updateList(mainmenu);
}

/*  power button handler
 *  toggles power on and off depending on battery % and if device is already in on or off state
 */
void MainWindow::on_powerButton_clicked()
{
    if (!powerOn && b.getPercentage() > 0){
        power_on();
    }
    else{
        power_off();
    }
}


/*  left button handler
 *  decreases power level if possible
 */
void MainWindow::on_leftButton_clicked()
{
    if (inTreatment == true) {
    power.decreasePower();
    if (electrodesConnected){
        b.setDegen(power.getPower());
    }
    // If in frequency menu, change label
    ui->power->setText(QString::number(power.getPower()));
    }
}

/*  right button handler
 *  increases power level if possible
 */
void MainWindow::on_rightButton_clicked()
{
    if (inTreatment == true) {
    power.increasePower();
    if (electrodesConnected){
        b.setDegen(power.getPower());
    }
    ui->power->setText(QString::number(power.getPower()));
    }
}

//
/*  timerFormat
 *  converts int to a stringed time format
 */
QString MainWindow::timerFormat(int time){
    QString theString = "";

    if(time >= 60){
        theString = QString("0%1:%2%3").arg(time/60).arg((time%60)/10).arg((time%60)%10);
    }
    else if(time <= 9){
        theString = QString("00:0%1").arg(time);
    }
    else {
        theString = QString("00:%1").arg(time);
    }

    return theString;
}
