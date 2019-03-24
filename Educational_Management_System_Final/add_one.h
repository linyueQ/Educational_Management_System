#ifndef ADD_ONE_H
#define ADD_ONE_H
#pragma once
#include <QWidget>
#include"base.h"
namespace Ui {
class add_one;
}

class add_one : public QWidget
{
    Q_OBJECT

public:
    explicit add_one(QWidget *parent = 0);
    ~add_one();

private slots:
    void on_comboBox_usertype_currentIndexChanged(int index);

    void on_pushButton_okin_clicked();

private:
    Ui::add_one *ui;
};

#endif // ADD_ONE_H
