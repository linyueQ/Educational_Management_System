#include "login.h"
#include "ui_login.h"


Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    ui->pw_in->setEchoMode(QLineEdit::Password);
    ui->no_in->setText(tr("admin"));
    ui->pw_in->setText(tr("admin"));
    ui->radioButton1_adm->setChecked(true);
    connect(ui->pw_in, SIGNAL(returnPressed()), ui->Button1_login, SIGNAL(clicked()), Qt::UniqueConnection);
    connect(ui->tel_in, SIGNAL(returnPressed()), ui->Button2_forgetpw, SIGNAL(clicked()), Qt::UniqueConnection);
    ui->Button1_login->setFocus();
    ui->Button1_login->setDefault(true);
    this->setMinimumSize(1, 1);
    this->setMaximumSize(950,480);
}

Login::~Login()
{
    delete ui;
}

//登陆按钮
void Login::on_Button1_login_clicked()
{
    QString ID=ui->no_in->text();
    QString PW=ui->pw_in->text();
    //判断是哪类用户登陆，好查询对应的数据库
    basic currentUser;//用于登录成功后记录当前用户的登陆信息
    if(ui->radioButton1_stu->isChecked())
    {
        //先尝试以学生身份登陆远程数据库
        if(connectDB(DBStudent,DBStudentPW))
        {
            QSqlQuery query;
            if(query.exec("select * from student where SNO = '"+ID.trimmed()+"'"))
            {
                if(query.next())
                {
                    currentUser.ID=query.value(0).toString().trimmed();
                    currentUser.name=query.value(1).toString().trimmed();
                    currentUser.sex=query.value(2).toString().trimmed();
                    currentUser.password=query.value(3).toString().trimmed();
                    currentUser.IDnumber=query.value(4).toString().trimmed();
                    currentUser.label=query.value(5).toString().trimmed();
                    currentUser.tel=query.value(6).toString().trimmed();
                    if(currentUser.password.compare(PW)==0)
                    {
                        //登陆成功,执行新建学生窗口，并关闭登陆窗口
                        //TODO 打开学生界面
                        s=new Student(currentUser);
                        QObject::connect(s, &Student::logout, this, &Login::show);
                        s->show();
                        this->close();
                    }
                    else
                    {
                        QMessageBox::information(NULL, "账户或密码错误！", "账户或密码错误！请重试！");

                    }
                }
                else
                {
                    QMessageBox::information(NULL, "不存在该账户！", "该账户不存在！请重试！");
                }
            }
            else
            {
                errorMessage=query.lastError().text();
                QMessageBox::information(NULL, "错误！", "执行比对数据库的时候发生错误，返回错误信息："+errorMessage);
            }

        }
        else
        {
            QMessageBox::information(NULL, "错误！", "无法连接到数据库，返回错误信息："+errorMessage);
        }
    }
    else if(ui->radioButton1_tea->isChecked())
    {
        //先尝试以老师身份登陆远程数据库
        if(connectDB(DBTeacher,DBTeacherPW))
        {
            QSqlQuery query;
            if(query.exec("select * from TEACHER where TNO = '"+ID.trimmed()+"'"))
            {
                if(query.next())
                {
                    currentUser.ID=query.value(0).toString().trimmed();
                    currentUser.name=query.value(1).toString().trimmed();
                    currentUser.sex=query.value(2).toString().trimmed();
                    currentUser.password=query.value(3).toString().trimmed();
                    currentUser.IDnumber=query.value(4).toString().trimmed();
                    currentUser.label=query.value(5).toString().trimmed();
                    currentUser.tel=query.value(6).toString().trimmed();
                    if(currentUser.password.compare(PW)==0)
                    {
                        //登陆成功,执行新建老师窗口，并关闭登陆窗口
                        //TODO 打开老师界面
                        t=new Teacher(currentUser);
                        QObject::connect(t, &Teacher::logout, this, &Login::show);
                        t->show();
                        this->close();
                    }
                    else
                    {
                        QMessageBox::information(NULL, "账户或密码错误！", "账户或密码错误！请重试！");

                    }
                }
                else
                {
                    QMessageBox::information(NULL, "不存在该账户！", "该账户不存在！请重试！");
                }
            }
            else
            {
                errorMessage=query.lastError().text();
                QMessageBox::information(NULL, "错误！", "执行比对数据库的时候发生错误，返回错误信息："+errorMessage);
            }
        }
        else
        {
            QMessageBox::information(NULL, "错误！", "无法连接到数据库，返回错误信息："+errorMessage);
        }
    }
    else if(ui->radioButton1_adm->isChecked())
    {
        //先尝试以管理员身份登陆远程数据库
        if(connectDB(DBAdmin,DBAdminPW))
        {
            QSqlQuery query;
            if(query.exec("select * from ADMIN where ANO = '"+ID.trimmed()+"'"))
            {
                if(query.next())
                {
                    currentUser.ID=query.value(0).toString().trimmed();
                    currentUser.name=query.value(1).toString().trimmed();
                    currentUser.sex=query.value(2).toString().trimmed();
                    currentUser.password=query.value(3).toString().trimmed();
                    currentUser.IDnumber=query.value(4).toString().trimmed();
                    currentUser.label=query.value(5).toString().trimmed();
                    currentUser.tel=query.value(6).toString().trimmed();
                    if(currentUser.password.compare(PW)==0)
                    {
                        //登陆成功,执行新建管理员窗口，并关闭登陆窗口
                        //TODO 打开管理员界面
                        a=new Admin(currentUser);
                        QObject::connect(a, &Admin::logout, this, &Login::show);
                        a->show();
                        this->close();
                    }
                    else
                    {
                        QMessageBox::information(NULL, "账户或密码错误！", "账户或密码错误！请重试！");

                    }
                }
                else
                {
                    QMessageBox::information(NULL, "不存在该账户！", "该账户不存在！请重试！");
                }
            }
            else
            {
                errorMessage=query.lastError().text();
                QMessageBox::information(NULL, "错误！", "执行比对数据库的时候发生错误，返回错误信息："+errorMessage);
            }

        }
        else
        {
            QMessageBox::information(NULL, "错误！", "无法连接到数据库，返回错误信息："+errorMessage);
        }
    }
    else
    {
        QMessageBox::information(NULL, "错误！", "你未选择任何用户类型！");

    }
}

//登陆tab的重置按钮
void Login::on_Button1_reset_clicked()
{

    ui->no_in->clear();

    ui->pw_in->clear();
}

//忘记密码tab的重置密码按钮
void Login::on_Button2_forgetpw_clicked()
{
    QString ID=ui->no_in_2->text();
    QString IDnum=ui->idnum_in->text();
    QString tel=ui->tel_in->text();
    //判断是哪类用户登陆，好查询对应的数据库

    if(ui->radioButton_stu->isChecked())
    {
        //先尝试以学生身份登陆远程数据库
        if(connectDB(DBAdmin,DBAdminPW))
        {
            QSqlQuery query;
            if(query.exec("select SID,STEL from student where SNO = '"+ID.trimmed()+"'"))
            {
                if(query.next())
                {
                    //重置密码的时候，要双重验证，验证身份证号和预留的手机号，两个同时正确才能通过重置密码的验证
                    if(query.value(0).toString().trimmed().compare(IDnum.trimmed())==0
                            &&query.value(1).toString().trimmed().compare(tel.trimmed())==0)
                    {
                        //验证成功，开始重置该用户的密码为账号后6位
                        query.clear();
                        bool pan=query.exec("update student set spsw= '"+ID.right(6)+"' where sno= '"+ID+"'");

                        if(pan)
                        {
                            QMessageBox::information(NULL, "成功！", "用户ID为 "+ID+"的用户密码已成功重置为ID后6位，请尽快修改密码");
                        }
                        else
                        {
                            errorMessage=query.lastError().text();
                            QMessageBox::information(NULL, "错误！", "在修改密码的时候发生错误，返回错误信息："+errorMessage);

                        }
                    }
                    else
                    {
                        QMessageBox::information(NULL, "输入的验证信息错误！", "你输入的信息有误！请重试！");

                    }
                }
                else
                {
                    QMessageBox::information(NULL, "不存在该账户！", "该账户不存在！请重试！");
                }
            }
            else
            {
                errorMessage=query.lastError().text();
                QMessageBox::information(NULL, "错误！", "执行比对数据库的时候发生错误，返回错误信息："+errorMessage);
            }

        }
        else
        {
            QMessageBox::information(NULL, "错误！", "无法连接到数据库，返回错误信息："+errorMessage);
        }
    }
    else if(ui->radioButton_tea->isChecked())
    {
        //先尝试以老师身份登陆远程数据库
        if(connectDB(DBAdmin,DBAdminPW))
        {
            QSqlQuery query;
            if(query.exec("select TID,TTEL from TEACHER where TNO = '"+ID.trimmed()+"'"))
            {
                if(query.next())
                {
                    //重置密码的时候，要双重验证，验证身份证号和预留的手机号，两个同时正确才能通过重置密码的验证
                    if(query.value(0).toString().trimmed().compare(IDnum.trimmed())==0
                            &&query.value(1).toString().trimmed().compare(tel.trimmed())==0)
                    {
                        //验证成功，开始重置该用户的密码为账号后6位
                        query.clear();
                        bool pan=query.exec("update teacher set tpsw= '"+ID.right(6)+"' where tno= '"+ID+"'");

                        if(pan)
                        {
                            QMessageBox::information(NULL, "成功！", "用户ID为 "+ID+"的用户密码已成功重置为ID后6位，请尽快修改密码");
                        }
                        else
                        {
                            errorMessage=query.lastError().text();
                            QMessageBox::information(NULL, "错误！", "在修改密码的时候发生错误，返回错误信息："+errorMessage);

                        }
                    }
                    else
                    {
                        QMessageBox::information(NULL, "输入的验证信息错误！", "你输入的信息有误！请重试！");

                    }
                }
                else
                {
                    QMessageBox::information(NULL, "不存在该账户！", "该账户不存在！请重试！");
                }
            }
            else
            {
                errorMessage=query.lastError().text();
                QMessageBox::information(NULL, "错误！", "执行比对数据库的时候发生错误，返回错误信息："+errorMessage);
            }

        }
        else
        {
            QMessageBox::information(NULL, "错误！", "无法连接到数据库，返回错误信息："+errorMessage);
        }
    }
    else if(ui->radioButton_adm->isChecked())
    {
        //先尝试以管理员身份登陆远程数据库
        if(connectDB(DBAdmin,DBAdminPW))
        {
            QSqlQuery query;
            if(query.exec("select AID,ATEL from ADMIN where ANO = '"+ID.trimmed()+"'"))
            {
                if(query.next())
                {
                    //重置密码的时候，要双重验证，验证身份证号和预留的手机号，两个同时正确才能通过重置密码的验证
                    if(query.value(0).toString().trimmed().compare(IDnum.trimmed())==0
                            &&query.value(1).toString().trimmed().compare(tel.trimmed())==0)
                    {
                        //验证成功，开始重置该用户的密码为账号后6位
                        query.clear();
                        bool pan=query.exec("update admin set apsw= '"+ID.right(6)+"' where ano= '"+ID+"'");

                        if(pan)
                        {
                            QMessageBox::information(NULL, "成功！", "用户ID为 "+ID+"的用户密码已成功重置为ID后6位，请尽快修改密码");
                        }
                        else
                        {
                            errorMessage=query.lastError().text();
                            QMessageBox::information(NULL, "错误！", "在修改密码的时候发生错误，返回错误信息："+errorMessage);

                        }
                    }
                    else
                    {
                        QMessageBox::information(NULL, "输入的验证信息错误！", "你输入的信息有误！请重试！");

                    }
                }
                else
                {
                    QMessageBox::information(NULL, "不存在该账户！", "该账户不存在！请重试！");
                }
            }
            else
            {
                errorMessage=query.lastError().text();
                QMessageBox::information(NULL, "错误！", "执行比对数据库的时候发生错误，返回错误信息："+errorMessage);
            }

        }
        else
        {
            QMessageBox::information(NULL, "错误！", "无法连接到数据库，返回错误信息："+errorMessage);
        }
    }
    else
    {
        QMessageBox::information(NULL, "错误！", "你未选择任何用户类型！");
    }
}

//忘记密码tab的重置按钮
void Login::on_Button2_reset_clicked()
{

    ui->idnum_in->clear();

    ui->no_in_2->clear();

    ui->tel_in->clear();

}
