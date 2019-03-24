#ifndef ADMIN_H
#define ADMIN_H
#pragma once
#include <QMainWindow>
#include <QMessageBox>

#include"base.h"
#include "add_all.h"
#include"add_one.h"
#include"welcome.h"
#include"query_all.h"
#include"addallmitable.h"
#include"addonemitable.h"
#include"add_schedule.h"
#include"make_query_gpa.h"
#include"query_all.h"
#include"query_delete_course.h"
#include"query_schedule.h"
#include"add_course.h"
#include"change_pw.h"
#include"querymi.h"
#include"inform_table.h"
#include"queryinform.h"

namespace Ui {
class Admin;
}

class Admin : public QMainWindow
{
    Q_OBJECT

public:
    explicit Admin(basic currentUser,QWidget *parent = 0);
    ~Admin();
    //从文本文件导入学生/老师信息，filepath是文本文件的路径
    bool InDatFromCSV(char filepath[300], int choice);
signals:
    void logout();
    //被用来做槽函数的都要加到这里来
private slots:

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_pushButton_clicked();

private:
    Ui::Admin *ui;
    Welcome *wel;
    add_one *a_o;
    add_all *a_a;
    query_all *q_a;

    add_course *a_c;
    query_delete_course *q_d_c;
    add_schedule *a_s;
    query_schedule *q_s;
    make_query_gpa *m_q_g;

    addOneMITable *mione;
    addAllMITable *miall;
    queryMI *querymi;
    Change_Password *c_pw;

    inform_Table *inform;
    queryInform *queryinform;

    basic currentUser;//记录当前登录用户的信息
};

#endif // ADMIN_H
