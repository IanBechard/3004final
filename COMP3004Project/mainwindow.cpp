#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->listWidget->addItem("Program");
    ui->listWidget->addItem("Frequency");
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

void MainWindow::menuHandler(QString s)
{
    std::vector<QString> programs{"program1", "program2", "program3"};
    std::vector<QString> frequencies{"frequency1", "frequency2", "frequency3"};
    if (s == "Programs"){
        updateList(programs);
    }
    else {
        updateList(frequencies);
    }
}

void MainWindow::on_selectButton_clicked()
{
    menuHandler(ui->listWidget->currentItem()->text());
}
