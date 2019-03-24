#ifndef WELCOME_H
#define WELCOME_H
#pragma once
#include <QWidget>
#include"base.h"
#include"informdetaildialog.h"
namespace Ui {
class Welcome;
}

class Welcome : public QWidget
{
    Q_OBJECT

public:
    explicit Welcome(QWidget *parent = 0);
    ~Welcome();
    Welcome(int userType,basic currentID,QWidget *parent = 0);
    void init();
private slots:
    void on_cilck_table(QTableWidgetItem *item);//点击了表格项需要执行的函数

    void on_pushButton_clicked();

private:
    Ui::Welcome *ui;
    basic currentUser;
    int userType;
    inform detail;
    informDetailDialog *IDD;
};

#endif // WELCOME_H
