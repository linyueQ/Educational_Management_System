#ifndef QUERY_SCHEDULE_H
#define QUERY_SCHEDULE_H
#pragma once

#include <base.h>

namespace Ui {
class query_schedule;
}

class query_schedule : public QWidget
{
    Q_OBJECT

public:
    explicit query_schedule(QWidget *parent = 0);
    ~query_schedule();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::query_schedule *ui;
    void flush_m();
};

#endif // QUERY_SCHEDULE_H
