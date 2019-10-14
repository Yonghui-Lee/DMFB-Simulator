#include "newpaint.h"
#include "defaultparameters.h"
#include <QPainter>
#include <QDebug>
#include <QLabel>
#include <QMessageBox>
QPoint Offset;

NewPaint::NewPaint(Info* info):info(info)
{

}

void NewPaint::drawDrip(QColor color,QPoint pos,Status statu){
    QPen pen(color);
    painter->setPen(pen);
    QBrush brush(color);
    painter->setBrush(brush);
    int len=sourceWide;
    //qDebug()<<"draw circle: "<<pos.x()-len<<" "<<pos.y()-len<<" "<<len*2,len*2;
    if(statu==full)
        painter->drawEllipse(Offset.x()+pos.x()-len,Offset.y()+pos.y()-len,len*2,len*2);
    else painter->drawEllipse(Offset.x()+pos.x()-len/2,Offset.y()+pos.y()-len/2,len,len);
}

void NewPaint::drawSlippingDrip(QColor color, QPoint pos, bool ishorizon){
    QPen pen(color);
    painter->setPen(pen);
    QBrush brush(color);
    painter->setBrush(brush);
    int len=sourceWide;
    if(ishorizon)
        painter->drawEllipse(Offset.x()+pos.x()-50,Offset.y()+pos.y()-len,100,len*2);
    else
        painter->drawEllipse(Offset.x()+pos.x()-len,Offset.y()+pos.y()-50,len*2,100);
}

void NewPaint::drawGridding(){
    painter->setRenderHint(QPainter::Antialiasing, true);
    QPen *pen = new QPen(QColor(210,113,4));
    pen->setWidth(penLen);
    painter->setPen(*pen);
    int gamehLen=DripLen*info->column;
    int gamevLen=DripLen*info->row;
    for (int i=0;i<=info->column;i++){
        painter->drawLine(Offset+QPoint(startY+i*DripLen,startX),Offset+QPoint(startY+i*DripLen,startX+gamevLen));
    }
    for (int i=0;i<=info->row;i++){
        painter->drawLine(Offset+QPoint(startY,startX+i*DripLen),Offset+QPoint(startY+gamehLen,startX+i*DripLen));
    }
}


void NewPaint::drawDrips(){
    painter->setRenderHint(QPainter::Antialiasing, true);
    for (int i=1;i<=info->row;i++){
        for (int j=1;j<=info->column;j++){



            QColor color=myColor[info->drips[info->time][i][j].color];
            QBrush brush(QColor(0,0,0));
          //  QRect rect=info->drips[i][j].getRect();

            if (info->drips[info->time][i][j].statu != clean){
                brush.setColor(QColor(color.red()*0.5,color.green()*0.5,color.blue()*0.5));
            }
            painter->setBrush(brush);


            if (info->drips[info->time][i][j].statu == full)
            {
                drawDrip(color,info->drips[info->time][i][j].getCenter(),full);
            }
            else if(info->drips[info->time][i][j].statu == polluted)
            {
                drawDrip(color,info->drips[info->time][i][j].getCenter(),polluted);
            }
            else if(info->drips[info->time][i][j].statu == hslipping)
            {
                drawSlippingDrip(color,info->drips[info->time][i][j].getCenter(),true);
            }
            else if(info->drips[info->time][i][j].statu == vslipping)
            {
                drawSlippingDrip(color,info->drips[info->time][i][j].getCenter(),false);
            }

         }
        }
    }


void NewPaint::drawPut(){
    painter->save();
    for(int i = 0;i<info->Input.size();i++)
    {
       painter->setPen(Qt::SolidLine);
       painter->setBrush(Qt::red);
       QFont font;
       font.setFamily("楷体");
       font.setPointSize(20);  // 字体大小
       font.setBold(true);     // 粗体
       painter->setFont(font);


        int x=info->Input[i].x();
        int y=info->Input[i].y();
        QPoint center = info->drips[info->time][y][x].getCenter();
        if(x==1&&y<=info->row)
        {
        QRectF rect(Offset.x()+center.x()-125,Offset.y()+center.y()-25,100,50);
        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignHCenter, "In");
        }
        else if(x==info->column&&y<=info->row)
        {
        QRectF rect(Offset.x()+center.x()+25,Offset.y()+center.y()-25,100,50);
        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignHCenter, "In");
        }
        else if(y==1&&x<=info->column)
        {
        QRectF rect(Offset.x()+center.x()-25,Offset.y()+center.y()-125,50,100);
        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignHCenter, "In");
        }
        else if(y==info->row&&x<=info->column)
        {
        QRectF rect(Offset.x()+center.x()-25,Offset.y()+center.y()+25,50,100);
        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignHCenter, "In");
        }
        else
        {
            info->Input.removeAll(QPoint(x,y));
            QMessageBox message(QMessageBox::Warning,"警告","输入端口应当在芯片的边界上。",QMessageBox::Ok,NULL);
            if (message.exec()==QMessageBox::Ok)
                {
                    qDebug()<<"clicked Ok\n";
                }
                else
                {
                    qDebug()<<"clicked no\n";
                }

        }

    }

    for(int i = 0;i<info->Output.size();i++)
    {
        painter->setPen(Qt::SolidLine);
        painter->setBrush(Qt::blue);
        QFont font;
        font.setFamily("楷体");
        font.setPointSize(15);  // 字体大小
        font.setBold(true);     // 粗体
        painter->setFont(font);

         int x=info->Output[i].x();
         int y=info->Output[i].y();
         QPoint center = info->drips[info->time][y][x].getCenter();
         if(x==1&&y<=info->row)
         {
         QRectF rect(Offset.x()+center.x()-125,Offset.y()+center.y()-25,100,50);
         painter->drawRect(rect);
         painter->drawText(rect, Qt::AlignHCenter, "Out");
         }
         else if(x==info->column&&y<=info->row)
         {
         QRectF rect(Offset.x()+center.x()+25,Offset.y()+center.y()-25,100,50);
         painter->drawRect(rect);
         painter->drawText(rect, Qt::AlignHCenter, "Out");
         }
         else if(y==1&&x<=info->column)
         {
         QRectF rect(Offset.x()+center.x()-25,Offset.y()+center.y()-125,50,100);
         painter->drawRect(rect);
         painter->drawText(rect, Qt::AlignHCenter, "Out");
         }
         else if(y==info->row&&x<=info->column)
         {
         QRectF rect(Offset.x()+center.x()-25,Offset.y()+center.y()+25,50,100);
         painter->drawRect(rect);
         painter->drawText(rect, Qt::AlignHCenter, "Out");
         }
         else
         {
          info->Output.removeAll(QPoint(x,y));
          QMessageBox message(QMessageBox::Warning,"警告","输出端口应当在芯片的边界上。",QMessageBox::Ok,NULL);
          if (message.exec()==QMessageBox::Ok)
              {
                  qDebug()<<"clicked Ok\n";
              }
              else
              {
                  qDebug()<<"clicked no\n";
              }
         }
    }

    painter->restore();
}

void NewPaint::drawNum()
{
    for (int i=1;i<=info->row;i++){
        for (int j=1;j<=info->column;j++){




            QRect rect=info->drips[0][i][j].getRect();

            painter->setPen(Qt::SolidLine);
            painter->setBrush(Qt::blue);
            QFont font;
            font.setFamily("楷体");
            font.setPointSize(15);  // 字体大小
            font.setBold(true);     // 粗体
            painter->setFont(font);

          int num = info->pollutecolor[i][j].size();

            painter->drawText(rect, Qt::AlignHCenter, QString::number(num));

         }
        }

}

void NewPaint::drawWashPut()
{
    painter->setPen(Qt::SolidLine);
    painter->setBrush(Qt::cyan);
    QFont font;
    font.setFamily("楷体");
    font.setPointSize(13);  // 字体大小
    font.setBold(true);     // 粗体
    painter->setFont(font);

     QPoint center1 = info->drips[0][1][1].getCenter();
     QRectF rect1(Offset.x()+center1.x()-125,Offset.y()+center1.y()-25,100,50);
     painter->drawRect(rect1);
     painter->drawText(rect1, Qt::AlignHCenter, "Wash\nInput");


     painter->setBrush(Qt::magenta);
     QPoint center2 = info->drips[0][info->row][info->column].getCenter();
     QRectF rect2(Offset.x()+center2.x()+25,Offset.y()+center2.y()-25,100,50);
     painter->drawRect(rect2);
     painter->drawText(rect2, Qt::AlignHCenter, "Waste");

}

void NewPaint::drawBarrier()
{
    for(int i=1;i<=this->info->row;i++)
        for(int j=1;j<=this->info->column;j++)
        {
            if(!info->washable[i][j])
            {

                painter->setPen(Qt::NoPen);
                painter->setBrush(QColor(60,60,60,50));
                QPoint center = info->drips[0][i][j].getCenter();
                QRectF rect(Offset.x()+center.x()-25,Offset.y()+center.y()-25,50,50);

                painter->drawRect(rect);
            }

        }
}

void NewPaint::drawWash()
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    drawDrip(QColor(152,245,255),
             info->drips[0][this->info->washstep[this->info->washtime-1].x()][this->info->washstep[this->info->washtime-1].y()].getCenter(),full);



}

void NewPaint::draw(){
    painter->setRenderHint(QPainter::Antialiasing, true);
    startX=30,startY=50;
    QPoint nowCenter(startY+DripLen*info->row/2,startX+DripLen*info->column/2);
    Offset=QPoint(0,0);//GameCenter-nowCenter;
    QPoint offset=GameCenter-nowCenter;
    startX+=offset.x();startY+=offset.y();


    drawGridding();
    drawDrips();
    drawPut();

    if(info->iswash)
    {drawWashPut();drawBarrier();}

    if(info->time==info->endtime&&info->endtime>0)
        drawNum();

    if(this->info->iswash&&this->info->washtime!=this->info->washstep.size())
        drawWash();

}
