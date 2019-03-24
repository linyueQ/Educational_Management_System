#ifndef IMPORT_DATA_THREAD_H
#define IMPORT_DATA_THREAD_H
#include<QThread>
#include"base.h"
class Import_Data_Thread : public QThread
{
    Q_OBJECT
public:
    explicit Import_Data_Thread(int choice ,QObject *parent = 0);

protected:
    virtual void run() Q_DECL_OVERRIDE;
private:
    int userType;

signals:
    void resultReady(int value);
};

#endif // IMPORT_DATA_THREAD_H
