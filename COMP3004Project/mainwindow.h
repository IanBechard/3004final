#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <programsmenu.h>
#include <frequencymenu.h>
#include <battery.h>
#include <qtimer.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    Battery b;
    bool powerOn = false;
    QTimer *timer = new QTimer(this);
    ~MainWindow();

private slots:
    void on_downButton_clicked();

    void on_upButton_clicked();

    void updateList(std::vector<QString> l);

    int validSelection(int x, int count);

    void on_selectButton_clicked();

    void selectMenuHandler(QString s);

    void backMenuHandler(QString s);

    void on_backButton_clicked();

    void updateCaption();

    void power_on();

    void power_off();

    void on_powerButton_clicked();

private:
    Ui::MainWindow *ui;
    frequencyMenu freqMenu;
    programsMenu progsMenu;

};

#endif // MAINWINDOW_H
