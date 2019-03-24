#ifndef MAKE_QUERY_GPA_H
#define MAKE_QUERY_GPA_H
#pragma once

#include"base.h"

namespace Ui {
class make_query_gpa;
}

class make_query_gpa : public QWidget
{
    Q_OBJECT

public:
    explicit make_query_gpa(QWidget *parent = 0);
    ~make_query_gpa();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::make_query_gpa *ui;
};

#endif // MAKE_QUERY_GPA_H
