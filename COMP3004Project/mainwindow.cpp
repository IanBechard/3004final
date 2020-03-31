#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    powerOn=true;
    toggleButtons();
    ui->listWidget->setStyleSheet("""QListWidget{background: white;}""");
    setMainMenu();
    ui->menuLabel->setText("Main Menu");
    on_downButton_clicked();
    power.setPower(0);
    electrodesConnected = false;
    ui->batteryTag->setText(QString::number(b.getPercentage()));
    ui->treatmentTag->setText("00:00");
    timer->start(100);//"tick" rate for battery
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
}

void MainWindow::toggleButtons(){
    if (powerOn){
    ui->backButton->setEnabled(true);
    ui->burgerButton->setEnabled(true);
    ui->upButton->setEnabled(true);
    ui->downButton->setEnabled(true);
    ui->leftButton->setEnabled(true);
    ui->rightButton->setEnabled(true);
    ui->selectButton->setEnabled(true);
    }
    else{
        ui->backButton->setEnabled(false);
        ui->burgerButton->setEnabled(false);
        ui->upButton->setEnabled(false);
        ui->downButton->setEnabled(false);
        ui->leftButton->setEnabled(false);
        ui->rightButton->setEnabled(false);
        ui->selectButton->setEnabled(false);
    }
}

void MainWindow::updateCaption(){
    b.degenerate();
    ui->batteryTag->setText(QString::number(b.getPercentage()));
    if (b.getPercentage() <= 0){
        power_off();
    }
}

void MainWindow::updateProgramTimer(){

    if(menu == "Frequencies"){
        if (updateMenuCounter >= freqMenu.getFrequencies()[menuType].getTime()){                                            //if the treatment has reached its end
            programTimer->stop();
        }
        else if (electrodesConnected == true && updateMenuCounter <= freqMenu.getFrequencies()[menuType].getTime()){       //if electrodes are on...
            updateMenuCounter++;
        }

        if((freqMenu.getFrequencies()[menuType].getTime()-updateMenuCounter) <= 9){
            test = QString("00:0%1").arg(freqMenu.getFrequencies()[menuType].getTime()-updateMenuCounter);
        }
        else {
            test = QString("00:%1").arg(freqMenu.getFrequencies()[menuType].getTime()-updateMenuCounter);
        }
    }
    else{//menu == "Programs"
        if (updateMenuCounter >= progsMenu.getPrograms()[menuType].getTime()){                                            //if the treatment has reached its end
            programTimer->stop();
        }
        else if (electrodesConnected == true && updateMenuCounter <= progsMenu.getPrograms()[menuType].getTime()){       //if electrodes are on...
            updateMenuCounter++;
        }

        if((progsMenu.getPrograms()[menuType].getTime()-updateMenuCounter) <= 9){
            test = QString("00:0%1").arg(progsMenu.getPrograms()[menuType].getTime()-updateMenuCounter);
        }
        else {
            test = QString("00:%1").arg(progsMenu.getPrograms()[menuType].getTime()-updateMenuCounter);
        }
    }

    ui->treatmentTag->setText(test);

}

MainWindow::~MainWindow()
{
    delete ui; //uh oh, stinky
}

void MainWindow::on_downButton_clicked()
{
    ui->listWidget->setCurrentRow(validSelection(ui->listWidget->currentRow() + 1, ui->listWidget->count()));
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
        programTimer->start(1000);

    }
    else if (s == "Pain"){
        updateMenuCounter = 0;
        menuType = 1;
        menu = "Programs";
        programTimer->start(1000);

    }
    else if (s == "CBT"){
        updateMenuCounter = 0;
        menuType = 2;
        menu = "Programs";
        programTimer->start(1000);

    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    else if (s == "1.0-9.9 Hz"){
        updateMenuCounter = 0;
        menuType = 0;
        menu = "Frequencies";
        programTimer->start(1000);

    }
    else if (s == "10 Hz"){
        updateMenuCounter = 0;
        menuType = 1;
        menu = "Frequencies";
        programTimer->start(1000);

    }
    else if (s == "20 Hz"){
        updateMenuCounter = 0;
        menuType = 2;
        menu = "Frequencies";
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
    else if(true/*in frequencies or programs*/)
    {
        ;
        //back out to programs or frequencies
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
}
void MainWindow::on_rightButton_clicked()
{
    power.increasePower();
    if (electrodesConnected){
        b.setDegen(power.getPower());
    }
}
