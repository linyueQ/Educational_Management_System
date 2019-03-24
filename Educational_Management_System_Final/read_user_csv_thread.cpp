#include "read_user_csv_thread.h"


Read_User_CSV::Read_User_CSV(QString path,int choice,QObject *parent)
    : QThread(parent)
{
    userType=choice;
    this->path=path;
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    string tmp = code->fromUnicode(path).data();
    strcpy(this->filepath, tmp.c_str());
}

void Read_User_CSV::run()
{
    int errorCode=InDatFromCSV(filepath,userType);
    emit resultReady(errorCode,this->path,userType);
}
