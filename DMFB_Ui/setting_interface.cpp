#include "setting_interface.h"
#include "ui_setting_interface.h"
#include "QLabel"
#include <QMessageBox>
#include<QDebug>

setting_interface::setting_interface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::setting_interface)
{
    ui->setupUi(this);
}

setting_interface::~setting_interface()
{
    delete ui;
}

void setting_interface::on_DMFBButton_clicked()
{
    if(ui->RowEdit->text().toInt()<=3&&ui->ColumnEdit->text().toInt()<=3)
    {/* QLabel *warning = new QLabel;
      QString strHTML = QString("<h1><font color = red>行数和列数不能同时小于等于3</font></h1>");
      warning->setText(strHTML);
      warning->setAlignment(Qt::AlignCenter);
      warning->setWindowTitle("WARNING");
      warning->show();
      */

        QMessageBox message(QMessageBox::Warning,"警告","行数和列数不能同时小于等于3",QMessageBox::Ok,NULL);
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
        emit sentDMFBchange(ui->RowEdit->text().toInt(),ui->ColumnEdit->text().toInt());

}

void setting_interface::on_InputAddButton_clicked()
{
emit sentInputChange(ui->InputAddX->text().toInt(),ui->InputAddY->text().toInt(),true);
}

void setting_interface::on_InputDeleteButton_clicked()
{
emit sentInputChange(ui->InputDeleteX->text().toInt(),ui->InputDeleteY->text().toInt(),false);
}

void setting_interface::on_OutputAddButton_clicked()
{
emit sentOutputChange(ui->OutputAddX->text().toInt(),ui->OutputAddY->text().toInt(),true);
}

void setting_interface::on_OutputDeleteButton_clicked()
{
emit sentOutputChange(ui->OutputDeleteX->text().toInt(),ui->OutputDeleteY->text().toInt(),false);
}
