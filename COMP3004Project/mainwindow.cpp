#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(batteryTimer, SIGNAL(timeout()), this, SLOT(updateCaption()));
    connect(programTimer, SIGNAL(timeout()), this, SLOT(updateProgramTimer()));
    power_off();
    //to be used for the battery class


}

//runs every second

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

void MainWindow::batteryBeep(){
        ui->batteryTag->setText("LOW BATTERY");


}

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

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_downButton_clicked()
{
    ui->listWidget->setCurrentRow(validSelection(ui->listWidget->currentRow() + 1, ui->listWidget->count()));
}

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

void MainWindow::on_homeButton_clicked(){
    menu = "";
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

void MainWindow::on_upButton_clicked()
{
    ui->listWidget->setCurrentRow(validSelection(ui->listWidget->currentRow() - 1, ui->listWidget->count()));
}

void MainWindow::on_selectButton_clicked()
{
    if (menu == "Frequencies") {
        // DO NOTHING
    }
    else {
     selectMenuHandler(ui->listWidget->currentItem()->text());
    }
}

void MainWindow::on_backButton_clicked()
{
    if (progOrFreq >= 1){
        backMenuHandler("");
    }
    else{
    backMenuHandler(ui->listWidget->currentItem()->text());
    }
}


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
void MainWindow::backMenuHandler(QString s)
{
    std::vector<QString> programs = progsMenu.getProgramsNames();
    std::vector<QString> frequencies  = freqMenu.getFrequenciesNames();

/*  ATTEMPTED FIX BY TRISTAN. SHOULD BE SOMETHING LIKE THIS FOR FINAL IMPLEMENTATION I JUST DON'T KNOW HOW THE count() FUNCTION WORKS.
    if (menu == "Frequencies") {
        menu = "";
        programTimer->stop();
        power.setPower(0);
        inTreatment=false;
        ui->timeLabel->clear();
        ui->count->clear();
        ui->powerLabel->clear();
        ui->power->clear();
        updateList(freqMenu.getFrequenciesNames());
        ui->menuLabel->setText("Frequency");
        //setheader to "Frequencies"
        on_downButton_clicked();
    }
    else if (menu == "Programs") {
        menu = "";
        programTimer->stop();
        power.setPower(0);
        inTreatment=false;
        ui->timeLabel->clear();
        ui->count->clear();
        ui->powerLabel->clear();
        ui->power->clear();
        updateList(progsMenu.getProgramsNames());
        ui->menuLabel->setText("Program");
        //setheader to "Programs"
        on_downButton_clicked();
    } */
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

void MainWindow::setMainMenu()
{
    std::vector<QString> mainmenu{"Program", "Frequency", "MED", "Screening", "Children", "Settings"};
    updateList(mainmenu);
}


void MainWindow::on_powerButton_clicked()
{
    if (!powerOn && b.getPercentage() > 0){
        power_on();
    }
    else{
        power_off();
    }
}


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
