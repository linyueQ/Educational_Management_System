#ifndef EDIT_USER_DIALOG_H
#define EDIT_USER_DIALOG_H

#include <QDialog>
#include"base.h"

namespace Ui {
class Edit_User_Dialog;
}

class Edit_User_Dialog : public QDialog
{
    Q_OBJECT
signals:
    void sendNewUser(basic newUser);
public:
    explicit Edit_User_Dialog(QWidget *parent = 0);
    //自定义一个构造函数，传入待编辑的用户类型，用户原来的信息，构造这个修改用户信息的对话框
    Edit_User_Dialog(int userType,basic edit_user,QWidget *parent = 0);
    ~Edit_User_Dialog();

    void accept();


private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::Edit_User_Dialog *ui;
    basic after_edit;
};

#endif // EDIT_USER_DIALOG_H
