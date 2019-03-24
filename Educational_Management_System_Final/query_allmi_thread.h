#ifndef QUERY_ALLMI_THREAD_H
#define QUERY_ALLMI_THREAD_H
#include<QThread>
#include"base.h"

class query_allmi_thread : public QThread
{
    Q_OBJECT
public:
    explicit query_allmi_thread(QString kw ,QObject *parent = 0);
protected:
    virtual void run() Q_DECL_OVERRIDE;
private:
    QString keyWords;
signals:
    void resultReady(int value);
};
#endif // QUERY_ALL_THREAD_H
