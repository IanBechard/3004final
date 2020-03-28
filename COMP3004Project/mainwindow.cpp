#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->listWidget->addItem("Program");
    ui->listWidget->addItem("Frequency");
    on_downButton_clicked();

    //to be used for the battery class
    QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(updateCaption()));
        timer->start(5000);

}

//runs every second
void MainWindow::updateCaption(){
    b.degenerate();
    ui->batteryTag->setText(QString::number(b.getPercentage()));
}
MainWindow::~MainWindow()
{
    delete ui;
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
        //setheader to "Programs"
        on_downButton_clicked();
    }
    else if (s == "Frequency"){
        updateList(freqMenu.getFrequenciesNames());
        //setheader to "Frequencies"
        on_downButton_clicked();
    }
}

void MainWindow::backMenuHandler(QString s)
{
    std::vector<QString> mainmenu{"Program", "Frequency"};
    std::vector<QString> programs = progsMenu.getProgramsNames();
    std::vector<QString> frequencies  = freqMenu.getFrequenciesNames();

    if (count(programs.begin(), programs.end(),  s) > 0 || count(frequencies.begin(), frequencies.end(),  s) > 0)
    {
        updateList(mainmenu);
        //set header to main menu
        on_downButton_clicked();
    }
    else if(true/*in frequencies or programs*/)
    {
        ;
        //back out to programs or frequencies
    }

}




