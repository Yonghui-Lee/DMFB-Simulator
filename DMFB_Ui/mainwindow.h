#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include"info.h"
#include"setting_interface.h"
#include"newpaint.h"
#include <QMainWindow>
#include <QTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void wash();


private slots:
    void on_setting_action_triggered();

    void on_fileopen_action_triggered();

    void DMFBchange(int now_row,int now_column);

    void InputChange(int row,int column,bool isadd);

    void OutputChange(int row,int column,bool isadd);

    void on_BeforeButton_clicked();

    void on_AfterButton_clicked();

    void on_RestartButton_clicked();

    void on_AutoButton_clicked();

    void onTimeOut();

    void onWashTimeOut();

    void on_wash_action_toggled(bool arg1);

private:
    Ui::MainWindow *ui;
    Info info;
    NewPaint *newpainter;
    QTimer *timer;
    QTimer *timer2;
};

#endif // MAINWINDOW_H
