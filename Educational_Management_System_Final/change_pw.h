#ifndef Change_Password_H
#define Change_Password_H

#include <QWidget>
#include"base.h"

namespace Ui {
class Change_Password;
}

class Change_Password : public QWidget
{
    Q_OBJECT

public:
    explicit Change_Password(basic currentUser,QWidget *parent = 0);
    ~Change_Password();

private slots:
    void on_Button_okchangepw_clicked();

    void on_Button_resetinput_clicked();

private:
    Ui::Change_Password *ui;
    basic currentUser;//记录需要修改密码的用户信息
};

#endif // Change_Password_H
