#ifndef DRIP_H
#define DRIP_H

#include <QColor>
#include <QRect>
#include <QPoint>
#include "defaultparameters.h"

//TODO:修改

enum Status{polluted,clean,full,hslipping,vslipping,forbidden};
class Drip
{
public:
    Drip();
    QPoint loc;//坐标
    int color;//颜色
    Status statu;
    QRect getRect();//Drip所在的矩形框坐标
    QPoint getCenter();//中心坐标
};

#endif // DRIP_H
