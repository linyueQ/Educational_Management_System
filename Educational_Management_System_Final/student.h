#ifndef STUDENT_H
#define STUDENT_H

#include <QMainWindow>

#include"base.h"
#include"change_pw.h"
#include"welcome.h"
#include"student_schedule.h"
#include"student_query_score.h"

namespace Ui {
class Student;
}

class Student : public QMainWindow
{
    Q_OBJECT

public:
    explicit Student(basic currentStudent,QWidget *parent = 0);
    ~Student();
signals:
    void logout();
private slots:

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_pushButton_clicked();

private:
    Ui::Student *ui;

    Welcome *wel;
    Change_Password *c_p;
    Student_Schedule *s_s;
    Student_Query_Score *s_q_s;
    basic currentUser;//记录当前登录的学生

};

#endif // STUDENT_H
