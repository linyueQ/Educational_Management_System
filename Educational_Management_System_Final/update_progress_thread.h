#ifndef UPDATE_PROGRESS_THREAD_H
#define UPDATE_PROGRESS_THREAD_H
#include <QThread>
#include"base.h"
class Update_Progress_Thread:public QThread
{
    Q_OBJECT
public:
    explicit Update_Progress_Thread(QObject *parent = 0);
protected:
    virtual void run() Q_DECL_OVERRIDE;
    ~Update_Progress_Thread();
signals:
    void resultReady(int value);
};

#endif // UPDATE_PROGRESS_THREAD_H
