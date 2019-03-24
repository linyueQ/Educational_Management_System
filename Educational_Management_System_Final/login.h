#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include"base.h"
#include"admin.h"
#include"teacher.h"
#include"student.h"

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void on_Button1_login_clicked();

    void on_Button1_reset_clicked();

    void on_Button2_forgetpw_clicked();

    void on_Button2_reset_clicked();

private:
    Ui::Login *ui;
    Admin *a;
    Teacher *t;
    Student *s;
};

#endif // LOGIN_H
