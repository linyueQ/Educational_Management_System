#ifndef ADD_COURSE_H
#define ADD_COURSE_H
#pragma once
#include "base.h"

namespace Ui {
class add_course;
}

class add_course : public QWidget
{
    Q_OBJECT

public:
    explicit add_course(QWidget *parent = 0);
    ~add_course();

private slots:
    void on_pushButton_clicked();//打开文件按钮的槽函数
    void on_pushButton_2_clicked();//确定导入的槽函数

private:
    Ui::add_course *ui;
};

#endif // ADD_COURSE_H
