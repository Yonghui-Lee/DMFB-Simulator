#ifndef INFO_H
#define INFO_H

#include<QVector>
#include<QSet>
#include"QPoint"
#include"drip.h"

class Info
{
public:
    Drip drips[110][16][16];

    int row,column;
    int endtime;
    int time;
    int warterTot; //water总数
    QSet<int> pollutecolor[16][16];
    QVector<QPoint> washstep;
    QVector<QPoint> Input;
    QVector<QPoint> Output;
    QSet<QString> Musicrecord[110];
    bool isautoplay;
    bool washable[16][16];
    bool haswarning;
    bool isconnect;
    bool iswash;
    int washtime;
    int dangeroustime;


    void changetime(bool isadd);
    bool BFS(QPoint startP,QPoint endP,QVector<QPoint>& shortestPath);
    bool getLoc(int &a,int &b,QPoint pos);
    void playmusiccontrol(int t);
    void init();
    void dangerdetect();
    void readtxt(QString filename);
    Info();
};

#endif // INFO_H
