#ifndef ADD_SCHEDULE_H
#define ADD_SCHEDULE_H
#pragma once

#include "base.h"

namespace Ui {
class add_schedule;
}

class add_schedule : public QWidget
{
    Q_OBJECT

public:
    explicit add_schedule(QWidget *parent = 0);
    ~add_schedule();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::add_schedule *ui;
    int make_SC();
};

#endif // ADD_SCHEDULE_H
