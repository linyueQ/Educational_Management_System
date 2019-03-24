
#ifndef TEACHERSEARCH_H
#define TEACHERSEARCH_H
#pragma once
#include<QWidget>
#include"base.h"

namespace Ui {
class teachersearch;
}

class teachersearch : public QWidget
{
    Q_OBJECT

public:
    explicit teachersearch(basic currtea,QWidget *parent = 0);
    ~teachersearch();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::teachersearch *ui;
    basic currTea;//记录当前登陆的老师
};

#endif // TEACHERSEARCH_H

