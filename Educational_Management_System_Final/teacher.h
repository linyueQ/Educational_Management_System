#ifndef TEACHER_H
#define TEACHER_H

#include <QMainWindow>

#include"base.h"
#include"teachersearch.h"
#include"studentlist.h"
#include"student_score.h"
#include"change_pw.h"
#include"welcome.h"
#include"inform.h"

namespace Ui {
class Teacher;
}

class Teacher : public QMainWindow
{
    Q_OBJECT

public:
    explicit Teacher(basic currentTeacher,QWidget *parent = 0);
    ~Teacher();
signals:
    void logout();
private slots:

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_pushButton_clicked();

private:
    Ui::Teacher *ui;
    teachersearch *a_1;
    studentlist *a_2;
    student_score *s_s;
    Welcome *wel;
    Change_Password *c_p;
    Inform *a_3;
    basic currentUser;//记录当前登录的老师
    //FormComm
};

#endif // TEACHER_H
