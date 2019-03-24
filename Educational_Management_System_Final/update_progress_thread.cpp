#include "update_progress_thread.h"

Update_Progress_Thread::Update_Progress_Thread(QObject *parent)
    : QThread(parent)
{

}

void Update_Progress_Thread::run()
{
    while(!isInterruptionRequested()&&progress<=100)
    {
        msleep(1);//休眠100ms

        emit resultReady(progress);
    }
}

Update_Progress_Thread::~Update_Progress_Thread()
{
    requestInterruption();
    quit();
    wait();
}
