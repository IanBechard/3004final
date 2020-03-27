#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
