#ifndef SETTING_INTERFACE_H
#define SETTING_INTERFACE_H

#include <QWidget>

namespace Ui {
class setting_interface;
}

class setting_interface : public QWidget
{
    Q_OBJECT

public:
    explicit setting_interface(QWidget *parent = 0);
    ~setting_interface();
signals:
    void sentDMFBchange(int now_row,int now_column);

    void sentInputChange(int row,int column,bool isadd);

    void sentOutputChange(int row,int column,bool isadd);

private slots:
    void on_DMFBButton_clicked();

    void on_InputAddButton_clicked();

    void on_InputDeleteButton_clicked();

    void on_OutputAddButton_clicked();

    void on_OutputDeleteButton_clicked();




private:
    Ui::setting_interface *ui;
};

#endif // SETTING_INTERFACE_H
