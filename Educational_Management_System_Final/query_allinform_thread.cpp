#include "query_allinform_thread.h"
query_allinform_thread::query_allinform_thread(QString kw,QObject *parent)
    : QThread(parent)
{
    keyWords=kw;
}

void query_allinform_thread::run()
{
    int errorCode=queryInformAll(keyWords);
    emit resultReady(errorCode);
}

