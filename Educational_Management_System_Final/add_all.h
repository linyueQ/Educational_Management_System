#ifndef ADD_ALL_H
#define ADD_ALL_H
#pragma once
#include <QWidget>
#include"base.h"
#include"import_data_thread.h"
#include"update_progress_thread.h"
#include"read_user_csv_thread.h"

namespace Ui {
class add_all;
}

class add_all : public QWidget
{
    //只有加入了Q_OBJECT，你才能使用QT中的signal和slot机制
    Q_OBJECT

public:
    /*explicit可以抑制内置类型隐式转换，即不同类型A对该类的对象进行赋值时，
    编译器会把这个类型A自动作为参数传到构造函数里面，在有默认参数的情况下，
    这样做是不安全的，而用了explicit就不会有这个问题，会直接报错提示*/
    explicit add_all(QWidget *parent = 0);
    ~add_all();


private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();
    void progressValue(int pvalue);

    void errorSum(int errorcode);

    void slot_after_readCSV(int errorCode,QString path,int type);

private:
    Ui::add_all *ui;
};

#endif // ADD_ALL_H
