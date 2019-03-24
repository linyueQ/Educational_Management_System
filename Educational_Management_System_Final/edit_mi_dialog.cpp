#include "edit_mi_dialog.h"
#include "ui_edit_mi_dialog.h"

edit_mi_dialog::edit_mi_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edit_mi_dialog)
{
    ui->setupUi(this);
}

edit_mi_dialog::~edit_mi_dialog()
{
    delete ui;
}

//重写点击ok时对话框结束的状态
void edit_mi_dialog::accept()
{
    emit sendNewMI(after_edit);
    QDialog::accept();

}

edit_mi_dialog::edit_mi_dialog(MI edit_mi,QWidget *parent):
    QDialog(parent),
    ui(new Ui::edit_mi_dialog)
{
    ui->setupUi(this);
    this->after_edit=edit_mi;//记录待修改学院的信息
    ui->label_MNAME->setText(edit_mi.MNAME);
    ui->lineEdit_MNO->setText(edit_mi.MNO);
    ui->lineEdit_IPLACE->setText(edit_mi.IPLACE);
    ui->lineEdit_INAME->setText(edit_mi.INAME);
    ui->lineEdit_INO->setText(edit_mi.INO);
    //qDebug()<<edit_mi.MNAME<<edit_mi.INAME;
}

void edit_mi_dialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->button(QDialogButtonBox::Ok)==button)
    {
        QSqlQuery query;
        after_edit.MNO=ui->lineEdit_MNO->text();
        after_edit.IPLACE=ui->lineEdit_IPLACE->text();
        after_edit.INO = ui->lineEdit_INO->text();        
        after_edit.INAME = ui->lineEdit_INAME->text();

        query.prepare("update MI set MNO=?,INO=?,INAME=?,IPLACE = ? where MNAME like ?");
        query.addBindValue(after_edit.MNO);
        query.addBindValue(after_edit.INO);
        query.addBindValue(after_edit.INAME);
        query.addBindValue(after_edit.IPLACE);
        query.addBindValue(after_edit.MNAME+"%");
        //qDebug()<<after_edit.MNAME<<"!!!"<<after_edit.MNO;
        if(!query.exec())
        {
            errorMessage=query.lastError().text();
            QMessageBox::information(NULL,"错误！","在更新数据库时发生错误，错误信息："+errorMessage);
        }
        else
        {
            QMessageBox::information(this,"修改成功","成功修改对应专业的信息！");
            this->close();
        }
    }

    else if(ui->buttonBox->button(QDialogButtonBox::Cancel)==button)
    {
        this->close();
    }
}
