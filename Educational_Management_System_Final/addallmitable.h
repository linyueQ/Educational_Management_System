#ifndef ADDALLMITABLE_H
#define ADDALLMITABLE_H
#pragma once
#include <QWidget>
#include"base.h"

namespace Ui {
class addAllMITable;
}

class addAllMITable : public QWidget
{
    Q_OBJECT

public:
    explicit addAllMITable(QWidget *parent = 0);
    ~addAllMITable();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::addAllMITable *ui;
};

#endif // ADDALLMITABLE_H
