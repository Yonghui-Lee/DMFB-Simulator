#ifndef NEWPAINT_H
#define NEWPAINT_H

#include"info.h"
#include <QPaintDevice>

class NewPaint
{
public:
    NewPaint(Info* info);
    void draw();//在device界面上按照info的信息绘制游戏界面
    void drawPut();
    void drawNum(); //结束后绘制污染次数
    void drawWashPut();
    void drawBarrier();
    void drawWash();
    void drawDrip(QColor color,QPoint pos,Status statu);
    void drawSlippingDrip(QColor color,QPoint pos,bool ishorizon);
    QPainter *painter;


private:
    void drawGridding();//画网格线
    void drawDrips();//画水滴
    QPaintDevice *device;
    Info *info;
};

#endif // NEWPAINT_H
