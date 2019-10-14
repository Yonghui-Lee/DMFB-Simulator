#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFile>
#include<QFileDialog>
#include<QDebug>
#include<QtGui>
#include<QKeyEvent>
#include<QPainter>
#include<QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new class QTimer(this);
    timer->start(1000);
    timer2 = new class QTimer(this);
    timer2->start(200);

    newpainter = new NewPaint(&info);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
    delete timer2;
    delete newpainter;
}



void MainWindow::mousePressEvent(QMouseEvent *event){
    qDebug()<<"mouse position:"<<event->pos();
    if (event->button() == Qt::LeftButton){
        int a,b;
        if (!info.getLoc(a,b,event->pos())) return;
        qDebug()<<"mouse press: "<<a<<" "<<b;
    if(this->info.iswash)
        this->info.washable[a][b]= !this->info.washable[a][b];
        this->update();
    }
}



void MainWindow::paintEvent(QPaintEvent *event){


     this->ui->lcdNumber->display(this->info.time);


    newpainter->painter = new QPainter(this);
    newpainter->draw();
if(this->info.isautoplay&&!(this->info.isconnect))
{connect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    this->info.isconnect=true;
}
if(this->info.time>=this->info.endtime||this->info.time>=this->info.dangeroustime)
{
    disconnect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    this->info.isautoplay=false;
    this->info.isconnect=false;
}

if(this->info.time==info.dangeroustime&&!this->info.haswarning)
    {

      QMessageBox message(QMessageBox::Warning,"警告","移动约束在下一时刻被打破",QMessageBox::Ok,NULL);
      this->info.haswarning = true;
      if (message.exec()==QMessageBox::Ok)
          {
            qDebug()<<"clicked Ok\n";
          }
      else
          {
            qDebug()<<"clicked no\n";
          }

   }

//清除pollute
 if(this->info.washtime!=this->info.washstep.size()&&this->info.iswash&&this->info.washtime>0)
 {   QPoint cleanP=this->info.washstep[this->info.washtime-1];
    if(this->info.drips[this->info.time][cleanP.x()][cleanP.y()].statu==polluted)
    {   int i=0;
        while(this->info.drips[this->info.time+i][cleanP.x()][cleanP.y()].statu==polluted
              &&this->info.drips[this->info.time+i][cleanP.x()][cleanP.y()].color==this->info.drips[this->info.time][cleanP.x()][cleanP.y()].color)
        {
            this->info.drips[this->info.time+i][cleanP.x()][cleanP.y()].statu=clean;
            i++;
        }
    }

  }


}

void MainWindow::onTimeOut()
{
    this->info.changetime(true);
    this->update();
}

void MainWindow::onWashTimeOut()
{
    this->info.washtime++;
    if(this->info.washtime==this->info.washstep.size())
           disconnect(timer2,SIGNAL(timeout()),this,SLOT(onWashTimeOut()));
    this->update();
}

void MainWindow::wash()
{  QThread::msleep(1000);
    if(this->info.drips[this->info.time][1][2].statu==full||this->info.drips[this->info.time][2][1].statu==full||
            this->info.drips[this->info.time][2][2].statu==full||!this->info.washable[1][1])
        return;
    this->info.washtime=0;
    this->info.washstep.clear();
    QVector<QPoint> needtowash;
    for(int i=1;i<=this->info.row;i++)
        for(int j=1;j<=this->info.column;j++)
            {
                if(this->info.drips[this->info.time][i][j].statu==polluted)
                    needtowash.append(QPoint(i,j));
                if(this->info.drips[this->info.time][i][j].statu==full&&this->info.drips[this->info.time-1][i][j].statu==polluted
                        &&this->info.drips[this->info.time][i][j].color !=this->info.drips[this->info.time-1][i][j].color)
                    {
                    QMessageBox message(QMessageBox::Warning,"警告","清洗失败",QMessageBox::Ok,NULL);
                    this->info.haswarning = true;
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
    if(needtowash.size()==0) return;
    needtowash.push_front(QPoint(1,1));
    needtowash.push_back(QPoint(this->info.row,this->info.column));
    int lastwash = 0;

    for(int i=1;i<needtowash.size();i++)
    {    QVector<QPoint> pathVec;
        if(this->info.BFS(needtowash[lastwash],needtowash[i],pathVec))
        {
            lastwash=i;
            this->info.washstep.append(pathVec);


        }

    }



    if(this->info.washstep.size()==0||this->info.washstep[this->info.washstep.size()-1]!=QPoint(this->info.row,this->info.column))
        return;
    connect(timer2,SIGNAL(timeout()),this,SLOT(onWashTimeOut()));
}

void MainWindow::on_setting_action_triggered()
{
 setting_interface* setting = new setting_interface;
 setting->setWindowTitle("设置");
 setting->setWindowModality(Qt::ApplicationModal);
 connect(setting,SIGNAL(sentDMFBchange(int,int)),this,SLOT(DMFBchange(int,int)));
 connect(setting,SIGNAL(sentInputChange(int,int,bool)),this,SLOT(InputChange(int,int,bool)));
 connect(setting,SIGNAL(sentOutputChange(int,int,bool)),this,SLOT(OutputChange(int,int,bool)));
 setting->show();
}

void MainWindow::on_fileopen_action_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                         tr("选择文件"),
                                         "",
                                         tr("Text files (*.txt)"));
    this->info.readtxt(filename);

    this->update();

   /*
    QFile f(filename);
    qDebug()<<filename;
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        qFatal("Could not open file");
    QTextStream in(&f);
    QString linestr;
    while(!in.atEnd())
    {   linestr=in.readLine();
        qDebug()<<linestr;
    }
    */
}

void MainWindow::DMFBchange(int now_row, int now_column)
{   this->info.Input.clear();
    this->info.Output.clear();



    for(int k=0;k<=101;k++)
        for (int i=1;i<=15;i++)
            for (int j=1;j<=15;j++){
                this->info.drips[k][i][j].color=0;
                this->info.drips[k][i][j].statu=clean;
                this->info.drips[k][i][j].loc=QPoint(i,j);
            }

     for(int k=0;k<=101;k++)
         this->info.Musicrecord[k].clear();

     for (int i=1;i<=15;i++)
         for (int j=1;j<=15;j++){
             this->info.washable[i][j]=true;
             this->info.pollutecolor[i][j].clear();
         }
        this->info.time = 0;
        this->info.isautoplay = false;
        this->info.warterTot = 0;
        this->info.isconnect = false;
        this->info. endtime=0;
        this->info.iswash = false;
        this->info.dangeroustime=200;
        this->info.haswarning = false;


        this->info.column = now_column;
        this->info.row = now_row;
        this->update();
        disconnect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));

        qDebug()<<this->info.column<<"*"<<this->info.row;

}

void MainWindow::InputChange(int row, int column, bool isadd)
{
  if(isadd)
        this->info.Input.append(QPoint(row,column));
  else
        this->info.Input.removeAll(QPoint(row,column));

  this->update();

  qDebug()<<this->info.Input;
}

void MainWindow::OutputChange(int row, int column, bool isadd)
{
    if(isadd)
            this->info.Output.append(QPoint(row,column));
    else
            this->info.Output.removeAll(QPoint(row,column));

    this->update();

  qDebug()<<this->info.Output;
}





void MainWindow::on_BeforeButton_clicked()
{if(info.time>0)
    {
        this->info.changetime(false);
        this->update();
    qDebug()<<"time:"<<this->info.time;
    }
}

void MainWindow::on_AfterButton_clicked()
{
    this->info.changetime(true);
    this->update();
    qDebug()<<"time:"<<this->info.time;

    if(this->info.iswash)
        wash();
}

void MainWindow::on_RestartButton_clicked()
{   this->info.iswash = false;
    this->ui->wash_action->setChecked(false);
    for (int i=1;i<=15;i++)
        for (int j=1;j<=15;j++){
            this->info.washable[i][j]=true;
        }
    this->info.time = 0;
    if(this->info.isconnect)
    {
        disconnect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
        this->info.isautoplay=false;
        this->info.isconnect=false;
    }
    this->update();
    qDebug()<<"time:"<<this->info.time;
}

void MainWindow::on_AutoButton_clicked()
{
    this->info.isautoplay = true;

}

void MainWindow::on_wash_action_toggled(bool arg1)
{
    if(arg1)
    {
        this->info.iswash = true;
        this->update();
    }
    else
    {

        this->info.iswash = false;
        for (int i=1;i<=15;i++)
            for (int j=1;j<=15;j++){
                this->info.washable[i][j]=true;
            }
        this->update();

    }

}
