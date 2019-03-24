#ifndef STUDENT_QUERY_SCORE_H
#define STUDENT_QUERY_SCORE_H

#include <QWidget>
#include"base.h"

namespace Ui {
class Student_Query_Score;
}

class Student_Query_Score : public QWidget
{
    Q_OBJECT

public:
    explicit Student_Query_Score(basic currentStudent,QWidget *parent = 0);
    ~Student_Query_Score();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Student_Query_Score *ui;
    basic currentStudent;
    bool isRefresh;//在把成绩表导出为CSV文件的时候需要先检查一下是否有刷新获取最新的成绩表
};

#endif // STUDENT_QUERY_SCORE_H
