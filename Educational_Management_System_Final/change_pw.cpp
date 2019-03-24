#include "change_pw.h"
#include"ui_change_pw.h"

Change_Password::Change_Password(basic currentUser,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Change_Password)
{
    this->currentUser=currentUser;
    ui->setupUi(this);
    ui->oldpw->setEchoMode(QLineEdit::Password);
    ui->newpw->setEchoMode(QLineEdit::Password);
    ui->renewpw->setEchoMode(QLineEdit::Password);
}

Change_Password::~Change_Password()
{
    delete ui;
}

//确认修改密码按钮
void Change_Password::on_Button_okchangepw_clicked()
{
    QSqlQuery query;

    //学生修改密码
    if(currentUser.ID.at(0)=='2')
    {
        if(query.exec("select SPSW from STUDENT where SNO = '"+currentUser.ID+"'"))
        {
            if(query.next())
            {
                //判断旧密码是不是正确
                if(query.value(0).toString().trimmed().compare(ui->oldpw->text().trimmed())==0)
                {
                    //还要判断一下重复两次的密码是不是一致
                    if(ui->newpw->text().trimmed().compare(ui->renewpw->text().trimmed())==0)
                    {
                        //旧密码匹配成功，把新密码写入到数据库
                        query.clear();
                        if(query.exec("update STUDENT set SPSW = '"+ui->newpw->text()+"' where SNO= '"+currentUser.ID.trimmed()+"'"))
                        {
                            QMessageBox::information(NULL, "修改密码成功！", "成功修改密码！请牢记新密码！");

                        }

                    }
                    else
                    {
                        QMessageBox::information(NULL, "重复输入新密码不匹配！", "两次输入的新密码不一致！请重试！");
                    }
                }
                else
                {
                    QMessageBox::information(NULL, "旧密码错误！", "您输入的旧密码有误！请重试！");
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
    //老师修改密码
    else if(currentUser.ID.at(0)=='1')
    {
        if(query.exec("select TPSW from TEACHER where TNO = '"+currentUser.ID+"'"))
        {
            if(query.next())
            {
                //判断旧密码是不是正确
                if(query.value(0).toString().trimmed().compare(ui->oldpw->text().trimmed())==0)
                {
                    //还要判断一下重复两次的密码是不是一致
                    if(ui->newpw->text().trimmed().compare(ui->renewpw->text().trimmed())==0)
                    {
                        //旧密码匹配成功，把新密码写入到数据库
                        query.clear();
                        if(query.exec("update TEACHER set TPSW = '"+ui->newpw->text()+"' where TNO= '"+currentUser.ID.trimmed()+"'"))
                        {
                            QMessageBox::information(NULL, "修改密码成功！", "成功修改密码！请牢记新密码！");

                        }

                    }
                    else
                    {
                        QMessageBox::information(NULL, "重复输入新密码不匹配！", "两次输入的新密码不一致！请重试！");
                    }
                }
                else
                {
                    QMessageBox::information(NULL, "旧密码错误！", "您输入的旧密码有误！请重试！");
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
    //管理员修改密码
    else if(currentUser.ID.at(0)=='3'||currentUser.ID.compare("admin")==0)
    {
        if(query.exec("select APSW from ADMIN where ANO = '"+currentUser.ID+"'"))
        {
            if(query.next())
            {
                //判断旧密码是不是正确
                if(query.value(0).toString().trimmed().compare(ui->oldpw->text().trimmed())==0)
                {
                    //还要判断一下重复两次的密码是不是一致
                    if(ui->newpw->text().trimmed().compare(ui->renewpw->text().trimmed())==0)
                    {
                        //旧密码匹配成功，把新密码写入到数据库
                        query.clear();
                        if(query.exec("update ADMIN set APSW = '"+ui->newpw->text()+"' where ANO= '"+currentUser.ID.trimmed()+"'"))
                        {
                            QMessageBox::information(NULL, "修改密码成功！", "成功修改密码！请牢记新密码！");
                        }

                    }
                    else
                    {
                        QMessageBox::information(NULL, "重复输入新密码不匹配！", "两次输入的新密码不一致！请重试！");
                    }
                }
                else
                {
                    QMessageBox::information(NULL, "旧密码错误！", "您输入的旧密码有误！请重试！");
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
        qDebug()<<"in change pw error"<<currentUser.ID<<endl;
    }

}

//修改密码框的重置输入按钮
void Change_Password::on_Button_resetinput_clicked()
{
    ui->newpw->clear();
    ui->oldpw->clear();
    ui->renewpw->clear();
}
