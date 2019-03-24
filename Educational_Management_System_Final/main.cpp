#include "admin.h"
#include<base.h>
#include <QApplication>
#include"login.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login l;
    l.show();

    return a.exec();
}
