#ifndef QUERY_ALL_THREAD_H
#define QUERY_ALL_THREAD_H
#include<QThread>
#include"base.h"

class query_all_thread : public QThread
{
    Q_OBJECT
public:
    explicit query_all_thread(int choice,QString kw ,QObject *parent = 0);
protected:
    virtual void run() Q_DECL_OVERRIDE;
private:
    int userType;
    QString keyWords;
signals:
    void resultReady(int value,int userType);
};
#endif // QUERY_ALL_THREAD_H
