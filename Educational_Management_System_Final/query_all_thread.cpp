#include "query_all_thread.h"
query_all_thread::query_all_thread(int choice,QString kw,QObject *parent)
    : QThread(parent)
{
    userType=choice;
    keyWords=kw;
}

void query_all_thread::run()
{
    int errorCode=queryAll(userType,keyWords);
    emit resultReady(errorCode,userType);
}
