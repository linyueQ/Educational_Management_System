#include "query_allmi_thread.h"
query_allmi_thread::query_allmi_thread(QString kw,QObject *parent)
    : QThread(parent)
{
    keyWords=kw;
}

void query_allmi_thread::run()
{
    int errorCode=queryMIAll(keyWords);
    emit resultReady(errorCode);
}

