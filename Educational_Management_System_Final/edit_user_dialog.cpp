#include "edit_user_dialog.h"
#include "ui_edit_user_dialog.h"

Edit_User_Dialog::Edit_User_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Edit_User_Dialog)
{
    ui->setupUi(this);
}

//重写点击ok时对话框结束的状态
void Edit_User_Dialog::accept()
{
    emit sendNewUser(after_edit);
    QDialog::accept();
}



//自定义的构造函数
Edit_User_Dialog::Edit_User_Dialog(int userType,basic edit_user,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Edit_User_Dialog)
{
    ui->setupUi(this);

    //把该用户原来的信息展示到对话框对应的位置
    ui->label_ID->setText(edit_user.ID);
    ui->lineEdit_name->setText(edit_user.name);
    ui->lineEdit_IDnum->setText(edit_user.IDnumber);
    ui->lineEdit_tel->setText(edit_user.tel);
    if(edit_user.sex[0]=='M')
        ui->comboBox_sex->setCurrentIndex(0);
    else
        ui->comboBox_sex->setCurrentIndex(1);

    QSqlQuery query;
    //下面进行填充年级和专业/学院的下拉框
    switch(userType)
    {
    case 0:
        ui->label_userType->setText("学生");
        ui->comboBox_label->clear();//填充之前先清除一下下拉框
        query.clear();
        query.exec("select distinct MNAME from MI");
        while(query.next())
        {
            ui->comboBox_label->addItem(query.value(0).toString().trimmed());
        }
        ui->comboBox_label->setCurrentText(edit_user.label);//设置显示当前学生的专业
        break;
    case 1:
        ui->label_userType->setText("老师");
        ui->comboBox_label->clear();//填充之前先清除一下下拉框
        query.clear();
        query.exec("select distinct INAME from MI");
        while(query.next())
        {
            ui->comboBox_label->addItem(query.value(0).toString().trimmed());
        }
        ui->comboBox_label->setCurrentText(edit_user.label);//设置显示当前老师的学院
        break;
    case 2:
        ui->label_userType->setText("管理员");
        ui->comboBox_label->clear();//填充之前先清除一下下拉框
        query.clear();
        query.exec("select distinct INAME from MI");
        while(query.next())
        {
            ui->comboBox_label->addItem(query.value(0).toString().trimmed());
        }
        ui->comboBox_label->setCurrentText(edit_user.label);//设置显示当前管理员的学院
        break;
    }

}

Edit_User_Dialog::~Edit_User_Dialog()
{
    delete ui;
}

//当修改用户信息的对话框的OK按钮或者Cancel按钮被按下的时候
void Edit_User_Dialog::on_buttonBox_clicked(QAbstractButton *button)
{
    //如果点击ok按钮
    if(ui->buttonBox->button(QDialogButtonBox::Ok)==button)
    {
        QSqlQuery query;
        after_edit;
        after_edit.ID=ui->label_ID->text();
        after_edit.name=ui->lineEdit_name->text();
        if(ui->comboBox_sex->currentIndex()==0)
            after_edit.sex="M";
        else
            after_edit.sex="F";
        after_edit.IDnumber=ui->lineEdit_IDnum->text();
        after_edit.label=ui->comboBox_label->currentText();
        after_edit.tel=ui->lineEdit_tel->text();

        //通过用户的ID的开头判断用户的类型
        if(after_edit.ID.at(0)=='2')
        {
            query.clear();
            query.prepare("update STUDENT set SNAME=?,SSEX=?,SID=?,MNAME=?,STEL=? where SNO like ?");
            query.addBindValue(after_edit.name);
            query.addBindValue(after_edit.sex);
            query.addBindValue(after_edit.IDnumber);
            query.addBindValue(after_edit.label);
            query.addBindValue(after_edit.tel);
            query.addBindValue(after_edit.ID+"%");
            qDebug()<<after_edit.ID<<after_edit.name<<after_edit.sex<<after_edit.IDnumber<<after_edit.label<<after_edit.tel<<endl;
            if(!query.exec())
            {
                errorMessage=query.lastError().text();
                QMessageBox::information(NULL,"错误！","在更新数据库时发生错误，错误信息："+errorMessage);
            }
        }
        else if(after_edit.ID.at(0)=='1')
        {
            query.clear();
            query.prepare("update TEACHER set TNAME=?,TSEX=?,TID=?,INAME=?,TTEL=? where TNO like ?");
            query.addBindValue(after_edit.name);
            query.addBindValue(after_edit.sex);
            query.addBindValue(after_edit.IDnumber);
            query.addBindValue(after_edit.label);
            query.addBindValue(after_edit.tel);
            query.addBindValue(after_edit.ID+"%");
            if(!query.exec())
            {
                errorMessage=query.lastError().text();
                QMessageBox::information(NULL,"错误！","在更新数据库时发生错误，错误信息："+errorMessage);
            }
        }
        else if(after_edit.ID.at(0)=='3')
        {
            query.clear();
            query.prepare("update ADMIN set ANAME=?,ASEX=?,AID=?,INAME=?,ATEL=? where ANO like ?");
            query.addBindValue(after_edit.name);
            query.addBindValue(after_edit.sex);
            query.addBindValue(after_edit.IDnumber);
            query.addBindValue(after_edit.label);
            query.addBindValue(after_edit.tel);
            query.addBindValue(after_edit.ID+"%");
            if(!query.exec())
            {
                errorMessage=query.lastError().text();
                QMessageBox::information(NULL,"错误！","在更新数据库时发生错误，错误信息："+errorMessage);
            }
        }
    }
    //如果点击cancel按钮
    else if(ui->buttonBox->button(QDialogButtonBox::Cancel)==button)
    {

    }
}
