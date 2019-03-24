#ifndef STUDENTLIST_H
#define STUDENTLIST_H

#include <QWidget>
#include"base.h"

namespace Ui {
class studentlist;
}

class studentlist : public QWidget
{
    Q_OBJECT

public:
    explicit studentlist(basic currtea,QWidget *parent = 0);
    ~studentlist();

private slots:
    void on_pushButton_clicked();
    void showstudentlist(QTableWidgetItem *item );



    void on_pushButton_2_clicked();

private:
    Ui::studentlist *ui;
    int select_row1;
    QString selected_NO1;
    basic currTea;//记录当前登录的老师
};

#endif // STUDENTLIST_H
