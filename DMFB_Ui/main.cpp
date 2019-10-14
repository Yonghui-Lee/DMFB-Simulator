#include "music.h"
#include "mainwindow.h"
#include <QApplication>

Music music;
int startX=30,startY=50;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setGeometry(50,50,1050,850);
    w.show();

    return a.exec();
}
