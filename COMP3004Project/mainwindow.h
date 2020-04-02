#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <programsmenu.h>
#include <frequencymenu.h>
#include <settingsmenu.h>
#include <battery.h>
#include <qtimer.h>
#include <QTime>
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
    bool flag;          // To debug the left/right buttons
    bool powerOn = false;
    bool electrodesConnected = false;
    PowerLevel power;
    QTimer *timer = new QTimer(this);
    QTimer *programTimer = new QTimer(this);
    QTime *programTime = new QTime(0,0,0);
    int updateMenuCounter = 0;
    int menuType = 100;
    QString menu = "";
    QString timerText = "";
    ~MainWindow();

public slots:
    void toggleButtons();
private slots:
    void on_downButton_clicked();

    void on_upButton_clicked();

    void updateList(std::vector<QString> l);

    int validSelection(int x, int count);

    void on_selectButton_clicked();

    void on_homeButton_clicked();

    void selectMenuHandler(QString s);

    void backMenuHandler(QString s);

    void on_backButton_clicked();

    void on_trodeButton_clicked();

    void updateCaption();

    void updateProgramTimer();

    void power_on();

    void power_off();

    void setMainMenu();

    void on_powerButton_clicked();

    void on_electrodesConnected_clicked();

    void on_leftButton_clicked();

    void on_rightButton_clicked();

    QString timerFormat(int time);

private:
    Ui::MainWindow *ui;
    frequencyMenu freqMenu;
    programsMenu progsMenu;
    settingsMenu settMenu;

};

#endif // MAINWINDOW_H
