#include "import_data_thread.h"

Import_Data_Thread::Import_Data_Thread(int choice,QObject *parent)
    : QThread(parent)
{
    userType=choice;

}

void Import_Data_Thread::run()
{
    int errorSum=writeInfoToDB(userType);

    emit resultReady(errorSum);
}
