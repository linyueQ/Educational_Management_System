#ifndef STUDENT_SCORE_H
#define STUDENT_SCORE_H

#include <QWidget>
#include"base.h"

namespace Ui {
class student_score;
}

class student_score : public QWidget
{
    Q_OBJECT

public:
    explicit student_score(basic currtea,QWidget *parent = 0);
    ~student_score();

private slots:

    //表格某条item内容改变要执行的函数，更新vector里面的成绩
    void tableChangeSignal(QTableWidgetItem *item);
    void on_pushButton_expandclass_clicked();

    void on_pushButton_okinscore_clicked();

    void showstudentscore(QTableWidgetItem *item );

    void on_pushButton_clicked();

private:
    Ui::student_score *ui;
    bool isEdit;//定义一个状态判断变量，true表示已经完成装载数据到tablewidget，false表示未完成装载数据，此时不触发修改vector里面的成绩
    basic currTea;//记录一下当前登录的老师
    int select_row1;//选中的班级的行数
};

#endif // STUDENT_SCORE_H
