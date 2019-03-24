#ifndef READ_USER_CSV_H
#define READ_USER_CSV_H
#include<QThread>
#include"base.h"
#include"add_all.h"
#include "ui_add_all.h"

class Read_User_CSV:public QThread
{
    Q_OBJECT
public:
    explicit Read_User_CSV(QString path,int choice,QObject *parent = 0);
protected:
    virtual void run() Q_DECL_OVERRIDE;
signals:
    void resultReady(int value,QString path,int type);
private:
    int userType;
    char filepath[300];
    QString path;

};
#endif // READ_USER_CSV_H
