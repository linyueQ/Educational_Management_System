#ifndef QUERY_DELETE_COURSE_H
#define QUERY_DELETE_COURSE_H
#pragma once

#include "base.h"

namespace Ui {
class query_delete_course;
}

class query_delete_course : public QWidget
{
    Q_OBJECT

public:
    explicit query_delete_course(QWidget *parent = 0);
    ~query_delete_course();

private slots:
    void on_pushButton_clicked();
    void menu_requested(QPoint pos);//右键显示菜单
    void del_course();//用来实现点击右键菜单的选项删除课程

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::query_delete_course *ui;
    QMenu* menu;
    QAction* del;
    QString sel_CNO;
};

#endif // QUERY_DELETE_COURSE_H
