#include "drip.h"
#include "defaultparameters.h"


Drip::Drip()
{
    loc=QPoint(0,0);
    color=0;
    statu = clean;

}



QPoint Drip::getCenter(){
    QRect rect=getRect();
    return QPoint(rect.x()+rect.width()/2,rect.y()+rect.height()/2);
}
QRect Drip::getRect(){
    return QRect(startY+DripLen*(loc.y()-1),startX+DripLen*(loc.x()-1),DripLen,DripLen);
}
