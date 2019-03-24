#ifndef INFORM_H
#define INFORM_H

#include <QWidget>
#include<QMessageBox>
#include<QFileDialog>
#include<QTextCodec>
#include<iostream>
#include<string>
#include<QByteArray>
#include<QTableWidget>
#include<QTableWidgetItem>
#include<QTextCodec>
#include <QDebug>
#include <QWidget>
#include"base.h"

namespace Ui {
class Inform;
}

class Inform : public QWidget
{
    Q_OBJECT

public:
    explicit Inform(basic currtea,QWidget *parent = 0);
    ~Inform();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::Inform *ui;
    basic currTea;
    int select_row1;
};

#endif // INFORM_H
