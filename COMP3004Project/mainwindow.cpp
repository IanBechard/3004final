#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCaption()));
    connect(programTimer, SIGNAL(timeout()), this, SLOT(updateProgramTimer()));
    power_off();
    //to be used for the battery class


}

//runs every second

void MainWindow::power_on(){
    flag=false;
    powerOn=true;
    toggleButtons();
    ui->timeLabel->clear();
    ui->count->clear();
    ui->powerLabel->clear();
    ui->power->clear();
    ui->listWidget->setStyleSheet("""QListWidget{background: white;}""");
    setMainMenu();
    ui->menuLabel->setText("Main Menu");
    on_downButton_clicked();
    power.setPower(0);
    electrodesConnected = false;
    ui->batteryTag->setText(QString::number(b.getPercentage()));
    ui->treatmentTag->setText("00:00");
    timer->start(5000);//"tick" rate for battery
}

void MainWindow::power_off(){
    powerOn=false;
    toggleButtons();

    ui->batteryTag->clear();
    ui->treatmentTag->clear();
    ui->listWidget->clear();
    ui->menuLabel->clear();
    ui->listWidget->setStyleSheet("""QListWidget{background: black;}""");
    timer->stop();
    programTimer->stop();
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
    }
    else{
        ui->backButton->setEnabled(false);
        ui->trodeButton->setEnabled(false);
        ui->upButton->setEnabled(false);
        ui->downButton->setEnabled(false);
        ui->leftButton->setEnabled(false);
        ui->rightButton->setEnabled(false);
        ui->selectButton->setEnabled(false);
    }
}

void MainWindow::updateCaption(){
    b.degenerate();
    ui->batteryTag->setText(QString::number(round(b.getPercentage())));
    if (b.getPercentage() <= 0){
        power_off();
    }
}

void MainWindow::updateProgramTimer(){

    if(menu == "Frequencies"){


         if (electrodesConnected == true){       //if electrodes are on...
            updateMenuCounter++;
            programTime->addSecs(1);

        }
        test = timerFormat(updateMenuCounter);
    }
    else{//menu == "Programs"
        if (updateMenuCounter >= progsMenu.getPrograms()[menuType].getTime()){                                            //if the treatment has reached its end
            programTimer->stop();
        }
        else if ((electrodesConnected) == (true && updateMenuCounter <= progsMenu.getPrograms()[menuType].getTime())){       //if electrodes are on...
            updateMenuCounter++;
            programTime->addSecs(-1);
        }

        test = timerFormat(progsMenu.getPrograms()[menuType].getTime()-updateMenuCounter);
    }

    ui->count->setText(test);

}

MainWindow::~MainWindow()
{
    delete ui; //uh oh, stinky
}

void MainWindow::on_downButton_clicked()
{
    ui->listWidget->setCurrentRow(validSelection(ui->listWidget->currentRow() + 1, ui->listWidget->count()));
}

void MainWindow::on_trodeButton_clicked()
{
    if (electrodesConnected == false)
    {
        electrodesConnected = true;
    }
    else {
     electrodesConnected = false;
    }
}


void MainWindow::on_upButton_clicked()
{
    ui->listWidget->setCurrentRow(validSelection(ui->listWidget->currentRow() - 1, ui->listWidget->count()));
}

void MainWindow::on_selectButton_clicked()
{
    selectMenuHandler(ui->listWidget->currentItem()->text());
}

void MainWindow::on_backButton_clicked()
{
    backMenuHandler(ui->listWidget->currentItem()->text());
    programTimer->stop();
    ui->treatmentTag->setText("00:00");
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
    else if (s == "Settings"){
        updateList(settMenu.getSettingsSNames());
        ui->menuLabel->setText("Settings");
        //setheader to "SettingsS"
        on_downButton_clicked();
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    else if (s == "Allergy"){
        updateMenuCounter = 0;
        menuType = 0;
        menu = "Programs";
        programTime->setHMS(0,0,(progsMenu.getPrograms()[menuType].getTime()));
        ui->treatmentTag->setText(timerFormat(progsMenu.getPrograms()[menuType].getTime()));
        ui->listWidget->clear();
        ui->timeLabel->setText("Time remaining: ");
        ui->count->setText(programTime->toString("m:ss"));
        programTimer->start(1000);

    }
    else if (s == "Pain"){
        updateMenuCounter = 0;
        menuType = 1;
        menu = "Programs";
        programTime->setHMS(0,0,(progsMenu.getPrograms()[menuType].getTime()));
        ui->treatmentTag->setText(timerFormat(progsMenu.getPrograms()[menuType].getTime()));
        ui->listWidget->clear();
        ui->timeLabel->setText("Time remaining: ");
        ui->count->setText(programTime->toString("m:ss"));
        programTimer->start(1000);

    }
    else if (s == "CBT"){
        updateMenuCounter = 0;
        menuType = 2;
        menu = "Programs";
        programTime->setHMS(0,0,(progsMenu.getPrograms()[menuType].getTime()));
        ui->treatmentTag->setText(timerFormat(progsMenu.getPrograms()[menuType].getTime()));
        ui->listWidget->clear();
        ui->timeLabel->setText("Time remaining: ");
        ui->count->setText(programTime->toString("m:ss"));
        programTimer->start(1000);

    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    else if (s == "1.0-9.9 Hz"){
        flag = true;
        updateMenuCounter = 0;
        menuType = 0;
        menu = "Frequencies";
        programTime->setHMS(0,0,0);
        ui->listWidget->clear();
        ui->timeLabel->setText("Time: ");
        ui->powerLabel->setText("Power: ");
        ui->power->setText("0");
        ui->count->setText(programTime->toString("mm:ss"));
        //ui->treatmentTag->setText(timerFormat(freqMenu.getFrequencies()[menuType].getTime()));
        programTimer->start(1000);

    }
    else if (s == "10 Hz"){
        flag = true;
        updateMenuCounter = 0;
        menuType = 1;
        menu = "Frequencies";
        programTime->setHMS(0,0,0);
        ui->listWidget->clear();
        ui->timeLabel->setText("Time: ");
        ui->powerLabel->setText("Power: ");
        ui->power->setText("0");
        ui->count->setText(programTime->toString("mm:ss"));
        //ui->treatmentTag->setText(timerFormat(freqMenu.getFrequencies()[menuType].getTime()));
        programTimer->start(1000);

    }
    else if (s == "20 Hz"){
        flag = true;
        updateMenuCounter = 0;
        menuType = 2;
        menu = "Frequencies";
        programTime->setHMS(0,0,0);
        ui->listWidget->clear();
        ui->timeLabel->setText("Time: ");
        ui->powerLabel->setText("Power: ");
        ui->power->setText("0");
        ui->count->setText(programTime->toString("mm:ss"));
        //ui->treatmentTag->setText(timerFormat(freqMenu.getFrequencies()[menuType].getTime()));
        programTimer->start(1000);

    }
}

void MainWindow::backMenuHandler(QString s)
{
    std::vector<QString> programs = progsMenu.getProgramsNames();
    std::vector<QString> frequencies  = freqMenu.getFrequenciesNames();
    std::vector<QString> settingsS  = settMenu.getSettingsSNames();

    if (count(programs.begin(), programs.end(),  s) > 0 || count(frequencies.begin(), frequencies.end(),  s) > 0 || count(settingsS.begin(), settingsS.end(),  s) > 0) // || count(settingsS.begin(), settingsS.end(),  s) > 0
    {
        setMainMenu();
        ui->menuLabel->setText("Main Menu");
        //set header to main menu
        on_downButton_clicked();
    }
    else if(s == "1.0-9.9 Hz" || s == "10 Hz" || s == "20 Hz")
    {
        programTimer->stop();
        updateList(progsMenu.getProgramsNames());
        ui->menuLabel->setText("Program");
        //setheader to "Programs"
        on_downButton_clicked();

        /* THINGS TO DO WHEN HEADED BACK TO PROGRAMS/FREQUENCIES
            ui->timeLabel->clear();
            ui->count->clear();
            ui->powerLabel->clear();
            ui->power->clear();
            flag = false;
        */
    }
    else {
        programTimer->stop();
        updateList(freqMenu.getFrequenciesNames());
        ui->menuLabel->setText("Frequency");
        //setheader to "Frequencies"
        on_downButton_clicked();
    }

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

void MainWindow::on_electrodesConnected_clicked()
{
    if (electrodesConnected){
        electrodesConnected = false;
        b.setDegen(1);
    }
    else{
        electrodesConnected = true;
        if (power.getPower() == 0){
            b.setDegen(1);
        }
        else{
        b.setDegen(power.getPower());
        }

    }

}

void MainWindow::on_leftButton_clicked()
{
    power.decreasePower();
    if (electrodesConnected){
        b.setDegen(power.getPower());
    }
    // If in frequency menu, change label
    if (flag == true) {
        ui->power->setText(QString::number(power.getPower()));
    }
}
void MainWindow::on_rightButton_clicked()
{
    power.increasePower();
    if (electrodesConnected){
        b.setDegen(power.getPower());
    }
    if (flag == true) {
        ui->power->setText(QString::number(power.getPower()));
    }
}

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
