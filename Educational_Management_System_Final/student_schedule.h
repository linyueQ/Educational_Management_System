#ifndef STUDENT_SCHEDULE_H
#define STUDENT_SCHEDULE_H

#include <QWidget>
#include"base.h"

namespace Ui {
class Student_Schedule;
}

class Student_Schedule : public QWidget
{
    Q_OBJECT

public:
    explicit Student_Schedule(basic currentStudent,QWidget *parent = 0);
    ~Student_Schedule();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Student_Schedule *ui;
    basic currentStudent;
    bool isRefresh;//在把课表导出为CSV文件的时候需要先检查一下是否有刷新获取最新的课表
};

#endif // STUDENT_SCHEDULE_H
