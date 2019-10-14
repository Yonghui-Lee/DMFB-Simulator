#include "music.h"
#include "info.h"
#include <QFile>
#include <QFileDialog>
#include <QtGui>
#include <QMessageBox>
#include <QDebug>
#include <QQueue>


Info::Info()
{
 row = 4;
 column = 4;
 init();


}

void Info::init()
{for(int k=0;k<=101;k++)
    for (int i=1;i<=15;i++)
        for (int j=1;j<=15;j++){
            this->drips[k][i][j].color=0;
            this->drips[k][i][j].statu=clean;
            this->drips[k][i][j].loc=QPoint(i,j);
        }

 for(int k=0;k<=101;k++)
     Musicrecord[k].clear();

 for (int i=1;i<=15;i++)
     for (int j=1;j<=15;j++){
         pollutecolor[i][j].clear();
         washable[i][j]=true;
     }
    Input.clear();
    Output.clear();
    time = 0;
    endtime = 0;
    washtime = 0;
    isautoplay = false;
    warterTot = 0;
    dangeroustime = 200;
    iswash = false;
    haswarning = false;
    isconnect = false;


}

void Info::playmusiccontrol(int t)
{
    QSet<QString>::const_iterator stlI = Musicrecord[t].constBegin();
        while(stlI != Musicrecord[t].constEnd()){

            music.playMusic(*stlI);
            ++stlI;
        }

}

bool Info::getLoc(int &a,int &b,QPoint pos){
       a=(pos.y()-startX)/DripLen+1;
       b=(pos.x()-startY)/DripLen+1;
       if (!(1<=a&&a<=row&&1<=b&&b<=column)){
           a=0;
           b=0;
           return false;
       }
       return true;
}


void Info::changetime(bool isadd)
{
   if(isadd)
   {
       if(time<this->endtime&&time<this->dangeroustime)
           this->time++;
   }
   else
   {
       if(time>0)
           this->time--;
   }

   this->playmusiccontrol(this->time);
}

void Info::readtxt(QString filename)
{
    for(int k=0;k<=101;k++)
        for (int i=1;i<=15;i++)
            for (int j=1;j<=15;j++){
                this->drips[k][i][j].color=0;
                this->drips[k][i][j].statu=clean;
                this->drips[k][i][j].loc=QPoint(i,j);
            }

     for(int k=0;k<=101;k++)
         Musicrecord[k].clear();

     for (int i=1;i<=15;i++)
         for (int j=1;j<=15;j++){
             pollutecolor[i][j].clear();
             washable[i][j]=true;
         }
        time = 0;
        isautoplay = false;
        warterTot = 0;
        isconnect = false;
        iswash =false;
        endtime=0;
        washtime = 0;
        haswarning = false;
        dangeroustime=200;


    QFile f(filename);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        qFatal("Could not open file");
    QTextStream in(&f);
    QString linestr;
    while(!in.atEnd())
    {
        linestr=in.readLine();
        linestr.chop(1);
        QStringList lines = linestr.split(" ");
        if(lines[0]==QString("Input"))
        { QStringList para = lines[1].split(",");
          int x = para[1].toInt();
          int y = para[2].toInt();

            if(this->Input.count(QPoint(x,y))==0)
            {
                this->isautoplay=false;
                this->init();

                QMessageBox message(QMessageBox::Warning,"警告","位置附近没有液滴的输入端口。",QMessageBox::Ok,NULL);
                if (message.exec()==QMessageBox::Ok)
                    {
                        qDebug()<<"clicked Ok\n";
                    }
                    else
                    {
                        qDebug()<<"clicked no\n";
                    }


            }
         else
            {   this->warterTot++;
                for(int i=para[0].toInt()+1;i<=101;i++)
                {

                    this->drips[i][y][x].color = this->warterTot;
                    this->drips[i][y][x].statu = full;

                }

            }

        }
        else if(lines[0]==QString("Move"))
        {


            QStringList para = lines[1].split(",");
            int x1 = para[1].toInt();
            int y1 = para[2].toInt();
            int x2 = para[3].toInt();
            int y2 = para[4].toInt();

            Musicrecord[para[0].toInt()+1].insert(QString("move"));

            for(int i=para[0].toInt()+1;i<=101;i++)
            {
                this->drips[i][y2][x2].statu = full;
                this->drips[i][y2][x2].color = this->drips[para[0].toInt()][y1][x1].color;

                this->drips[i][y1][x1].statu = polluted;


            }
        }
        else if(lines[0]==QString("Split"))
        {QStringList para = lines[1].split(",");
            int x1 = para[1].toInt();
            int y1 = para[2].toInt();
            int x2 = para[3].toInt();
            int y2 = para[4].toInt();
            int x3 = para[5].toInt();
            int y3 = para[6].toInt();

            Musicrecord[para[0].toInt()+1].insert(QString("splitbegin"));
            Musicrecord[para[0].toInt()+2].insert(QString("splitend"));

            if(y2==y3) this->drips[para[0].toInt()+1][y1][x1].statu = hslipping;
            else this->drips[para[0].toInt()+1][y1][x1].statu = vslipping;

            warterTot +=2;

            for(int i=para[0].toInt()+1+1;i<=101;i++)
            {
                this->drips[i][y1][x1].statu = polluted;


                this->drips[i][y2][x2].statu = full;
                this->drips[i][y2][x2].color = this->warterTot-1;

                this->drips[i][y3][x3].statu = full;
                this->drips[i][y3][x3].color = this->warterTot;

            }



        }
        else if(lines[0]==QString("Mix"))
        {
            QStringList para = lines[1].split(",");
            int n = para.size()/2;

            for(int i=0;i<n-1;i++)
            {
                Musicrecord[para[0].toInt()+1+i].insert(QString("move"));
                int colornow = this->drips[para[0].toInt()][para[2].toInt()][para[1].toInt()].color;
                this->drips[para[0].toInt()+1+i][para[2*i+4].toInt()][para[2*i+3].toInt()].color = colornow;
                this->drips[para[0].toInt()+1+i][para[2*i+4].toInt()][para[2*i+3].toInt()].statu = full;
                for(int j=para[0].toInt()+1+i;j<=100;j++)
                {
                    this->drips[j][para[2*i+2].toInt()][para[2*i+1].toInt()].statu = polluted;
                    this->drips[j][para[2*i+2].toInt()][para[2*i+1].toInt()].color = colornow;
                }
            }
            for(int i=para[0].toInt()+1;i<=100;i++)
            {
                this->drips[i][para[2].toInt()][para[1].toInt()].statu = polluted;
            }
            for(int j=para[0].toInt()+n-1;j<=100;j++)
            {
                this->drips[j][para[2*n].toInt()][para[2*n-1].toInt()].statu = full;
                this->drips[j][para[2*n].toInt()][para[2*n-1].toInt()].color = this->drips[j-1][para[2*n].toInt()][para[2*n-1].toInt()].color;
            }



        }
        else if(lines[0]==QString("Merge"))
        {
            QStringList para = lines[1].split(",");
            int x1 = para[1].toInt();
            int y1 = para[2].toInt();
            int x2 = para[3].toInt();
            int y2 = para[4].toInt();

            Musicrecord[para[0].toInt()+2].insert(QString("merge"));
            this->warterTot ++;

            if(y1==y2) this->drips[para[0].toInt()+1][(y1+y2)/2][(x1+x2)/2].statu = hslipping;
            else this->drips[para[0].toInt()+1][(y1+y2)/2][(x1+x2)/2].statu = vslipping;

            this->drips[para[0].toInt()+1][(y1+y2)/2][(x1+x2)/2].color = this->warterTot;


            for(int i=para[0].toInt()+2;i<=101;i++)
            {
                this->drips[i][(y1+y2)/2][(x1+x2)/2].statu = full;
                this->drips[i][(y1+y2)/2][(x1+x2)/2].color = this->warterTot;

                this->drips[i][y1][x1].statu = polluted;

                this->drips[i][y2][x2].statu = polluted;


            }


        }
        else if(lines[0]==QString("Output"))
        {

            QStringList para = lines[1].split(",");
            int x = para[1].toInt();
            int y = para[2].toInt();

            if(para[0].toInt()+1>this->endtime)
                endtime=para[0].toInt()+1;

            if(this->Output.count(QPoint(x,y))==0)
            {

                this->isautoplay=false;
                this->init();

                QMessageBox message(QMessageBox::Warning,"警告","位置附近没有液滴的输出端口。",QMessageBox::Ok,NULL);
                if (message.exec()==QMessageBox::Ok)
                    {
                        qDebug()<<"clicked Ok\n";
                    }
                    else
                    {
                        qDebug()<<"clicked no\n";
                    }
            }
            else
            {
              for(int i=para[0].toInt()+1;i<=101;i++)
              {
                this->drips[i][y][x].statu = polluted;

              }
           }
        }


    }

    for(int i=0;i<=endtime&&i<=dangeroustime;i++)
        for(int j=1;j <= row ;j++)
            for(int k=1;k <= column;k++)
            {
                if(this->drips[i][j][k].color!=0)
                    pollutecolor[j][k].insert(this->drips[i][j][k].color);


             //危险探测
                if(this->drips[i+1][j][k].statu == full)
                {


                    if(this->drips[i+1][j-1][k-1].statu==full||this->drips[i+1][j][k-1].statu==full||
                            this->drips[i+1][j+1][k-1].statu==full||this->drips[i+1][j+1][k].statu==full||
                            this->drips[i+1][j+1][k+1].statu==full||this->drips[i+1][j][k+1].statu==full||
                            this->drips[i+1][j-1][k+1].statu==full||this->drips[i+1][j-1][k].statu==full
                            )
                    {dangeroustime = i; qDebug()<<"dageroustime:"<<dangeroustime<<endl<<i<<" "<<j<<" "<<k; return;}

                    if((this->drips[i+2][j-1][k-1].statu==full&&this->drips[i+1][j][k].color!=this->drips[i+2][j-1][k-1].color&&this->drips[i+1][j-1][k-1].statu!=hslipping&&this->drips[i+1][j-1][k-1].statu!=vslipping)||
                            (this->drips[i+2][j][k-1].statu==full&&this->drips[i+1][j][k].color!=this->drips[i+2][j][k-1].color&&this->drips[i+1][j][k-1].statu!=hslipping&&this->drips[i+1][j][k-1].statu!=vslipping)||
                            (this->drips[i+2][j+1][k-1].statu==full&&this->drips[i+1][j][k].color!=this->drips[i+2][j+1][k-1].color&&this->drips[i+1][j+1][k-1].statu!=hslipping&&this->drips[i+1][j+1][k-1].statu!=vslipping)||
                            (this->drips[i+2][j+1][k].statu==full&&this->drips[i+1][j][k].color!=this->drips[i+2][j+1][k].color&&this->drips[i+1][j+1][k].statu!=hslipping&&this->drips[i+1][j+1][k].statu!=vslipping)||
                            (this->drips[i+2][j+1][k+1].statu==full&&this->drips[i+1][j][k].color!=this->drips[i+2][j+1][k+1].color&&this->drips[i+1][j+1][k+1].statu!=hslipping&&this->drips[i+1][j+1][k+1].statu!=vslipping)||
                            (this->drips[i+2][j][k+1].statu==full&&this->drips[i+1][j][k].color!=this->drips[i+2][j][k+1].color&&this->drips[i+1][j][k+1].statu!=hslipping&&this->drips[i+1][j][k+1].statu!=vslipping)||
                            (this->drips[i+2][j-1][k+1].statu==full&&this->drips[i+1][j][k].color!=this->drips[i+2][j-1][k+1].color&&this->drips[i+1][j-1][k+1].statu!=hslipping&&this->drips[i+1][j-1][k+1].statu!=vslipping)||
                            (this->drips[i+2][j-1][k].statu==full&&this->drips[i+1][j][k].color!=this->drips[i+2][j-1][k].color&&this->drips[i+1][j-1][k].statu!=hslipping&&this->drips[i+1][j-1][k].statu!=vslipping)
                            )
                    {dangeroustime = i+1;qDebug()<<"dageroustime:"<<dangeroustime<<endl<<i<<" "<<j<<" "<<k;  return;}

               }


            }


}


bool Info::BFS(QPoint startP,QPoint endP,QVector<QPoint>& shortestPath)
{
        if (startP == endP) { //起点即终点
            shortestPath.push_back(startP);
            return true;
        }

        //mark标记每一个节点的前驱节点，如果没有则为（-1，-1），如果有，则表示已经被访问
        QPoint mark[16][16];
        for(int i=0;i<16;i++)
            for(int j=0;j<16;j++)
                mark[i][j]==QPoint(-1,-1);



        QQueue<QPoint> queuePoint;
        queuePoint.push_back(startP);
        //将起点的前驱节点设置为自己
        mark[startP.x()][startP.y()] = startP;

        while (queuePoint.empty() == false) {
            QPoint pointFront = queuePoint.front();
            queuePoint.pop_front();

            if (pointFront.x() - 1 > 0 && this->washable[pointFront.x() - 1][pointFront.y()]
                    &&!(this->drips[this->time][pointFront.x() - 1][pointFront.y()].statu==full||this->drips[this->time][pointFront.x() - 1-1][pointFront.y()-1].statu==full||
                      this->drips[this->time][pointFront.x() - 1][pointFront.y()-1].statu==full||this->drips[this->time][pointFront.x() - 1+1][pointFront.y()-1].statu==full||
                      this->drips[this->time][pointFront.x() - 1+1][pointFront.y()].statu==full||this->drips[this->time][pointFront.x() - 1+1][pointFront.y()+1].statu==full||
                      this->drips[this->time][pointFront.x() - 1][pointFront.y()+1].statu==full||this->drips[this->time][pointFront.x() - 1-1][pointFront.y()+1].statu==full||
                      this->drips[this->time][pointFront.x() - 1-1][pointFront.y()].statu==full)
                    ) {//上节点连通
                if (mark[pointFront.x() - 1][pointFront.y()] == QPoint(0,0)) {//上节点未被访问，满足条件，如队列
                    mark[pointFront.x() - 1][pointFront.y()] = pointFront;
                    queuePoint.push_back(QPoint(pointFront.x() - 1, pointFront.y())); //入栈
                    if (QPoint(pointFront.x() - 1, pointFront.y()) == endP) { //找到终点
                        break;
                    }
                }
            }

            if (pointFront.x() + 1 <= this->row && this->washable[pointFront.x()+1][pointFront.y()]
                    &&
                   !(this->drips[this->time][pointFront.x() + 1][pointFront.y()].statu==full||this->drips[this->time][pointFront.x()+1-1][pointFront.y()-1].statu==full||
                     this->drips[this->time][pointFront.x()+1][pointFront.y()-1].statu==full||this->drips[this->time][pointFront.x()+1+1][pointFront.y()-1].statu==full||
                     this->drips[this->time][pointFront.x()+1+1][pointFront.y()].statu==full||this->drips[this->time][pointFront.x()+1+1][pointFront.y()+1].statu==full||
                      this->drips[this->time][pointFront.x()+1][pointFront.y()+1].statu==full||this->drips[this->time][pointFront.x()+1-1][pointFront.y()+1].statu==full||
                      this->drips[this->time][pointFront.x()+1-1][pointFront.y()].statu==full)
                    ) {//右节点连通
                if (mark[pointFront.x() + 1][pointFront.y()] == QPoint(0,0)) {//右节点未被访问，满足条件，如队列
                    mark[pointFront.x() + 1][pointFront.y()] = pointFront;
                    queuePoint.push_back(QPoint(pointFront.x() + 1, pointFront.y()));    //入栈
                    if (QPoint(pointFront.x() + 1, pointFront.y()) == endP) { //找到终点
                        break;
                    }
                }
            }

            if (pointFront.y() + 1 <= this->column && this->washable[pointFront.x()][pointFront.y()+1]
                    &&
                    !(this->drips[this->time][pointFront.x()][pointFront.y()+1].statu==full||this->drips[this->time][pointFront.x()-1][pointFront.y()+1-1].statu==full||
                      this->drips[this->time][pointFront.x()][pointFront.y()+1-1].statu==full||this->drips[this->time][pointFront.x()+1][pointFront.y()+1-1].statu==full||
                      this->drips[this->time][pointFront.x()+1][pointFront.y()+1].statu==full||this->drips[this->time][pointFront.x()+1][pointFront.y()+1+1].statu==full||
                     this->drips[this->time][pointFront.x()][pointFront.y()+1+1].statu==full||this->drips[this->time][pointFront.x()-1][pointFront.y()+1+1].statu==full||
                      this->drips[this->time][pointFront.x()-1][pointFront.y()+1].statu==full)
                    ) {//下节点连通
                if (mark[pointFront.x()][pointFront.y() + 1] == QPoint(0,0)) {//下节点未被访问，满足条件，如队列
                    mark[pointFront.x()][pointFront.y() + 1] = pointFront;
                    queuePoint.push_back(QPoint(pointFront.x(), pointFront.y() + 1));    //入栈
                    if (QPoint(pointFront.x(), pointFront.y() + 1) == endP) { //找到终点
                        break;
                    }
                }
            }

            if (pointFront.y() - 1 > 0 && this->washable[pointFront.x()][pointFront.y() - 1]
                    &&
                  !(this->drips[this->time][pointFront.x()][pointFront.y()-1].statu==full||this->drips[this->time][pointFront.x()-1][pointFront.y()-1-1].statu==full||
                     this->drips[this->time][pointFront.x()][pointFront.y()-1-1].statu==full||this->drips[this->time][pointFront.x()+1][pointFront.y()-1-1].statu==full||
                    this->drips[this->time][pointFront.x()+1][pointFront.y()-1].statu==full||this->drips[this->time][pointFront.x()+1][pointFront.y()-1+1].statu==full||
                      this->drips[this->time][pointFront.x()][pointFront.y()-1+1].statu==full||this->drips[this->time][pointFront.x()-1][pointFront.y()-1+1].statu==full||
                    this->drips[this->time][pointFront.x()-1][pointFront.y()-1].statu==full)
                    ) {//左节点连通
                if (mark[pointFront.x()][pointFront.y() - 1] == QPoint(0,0)) {//上节点未被访问，满足条件，如队列
                    mark[pointFront.x()][pointFront.y() - 1] = pointFront;
                    queuePoint.push_back(QPoint(pointFront.x(), pointFront.y() - 1));    //入栈
                    if (QPoint(pointFront.x(), pointFront.y() - 1) == endP) { //找到终点
                        break;
                    }
                }
            }
        }
        if (queuePoint.empty() == false) {
            int row = endP.x();
            int col = endP.y();
            shortestPath.push_back(endP);
            while (!(mark[row][col] == startP)) {
                shortestPath.push_front(mark[row][col]);
                int row1=row;int col1 = col;
                row = mark[row1][col1].x();
                col = mark[row1][col1].y();
            }
            shortestPath.push_front(startP);
            return true;
        }
        else return false;


}

