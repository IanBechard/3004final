#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <programsmenu.h>
#include <frequencymenu.h>
#include <settingsmenu.h>
#include <battery.h>
#include <qtimer.h>
#include <powerlevel.h>


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
    bool electrodesConnected = false;
    PowerLevel power;
    QTimer *timer = new QTimer(this);
    QTimer *programTimer = new QTimer(this);
    int updateMenuCounter = 0;
    ~MainWindow();

public slots:
    void toggleButtons();
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

    void updateProgramTimer();

    void power_on();

    void power_off();

    void setMainMenu();

    void on_powerButton_clicked();

    void on_electrodesConnected_clicked();

    void on_leftButton_clicked();

    void on_rightButton_clicked();

private:
    Ui::MainWindow *ui;
    frequencyMenu freqMenu;
    programsMenu progsMenu;
    settingsMenu settMenu;

};

#endif // MAINWINDOW_H
