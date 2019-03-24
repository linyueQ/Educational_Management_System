#include "informdetaildialog.h"
#include "ui_informdetaildialog.h"
#include <QRegExp>

informDetailDialog::informDetailDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::informDetailDialog)
{
    ui->setupUi(this);
}

informDetailDialog::~informDetailDialog()
{
    delete ui;
}
informDetailDialog::informDetailDialog(inform detail,QWidget *parent)
    :QDialog(parent),
      ui(new Ui::informDetailDialog)
{
    ui->setupUi(this);
    QString res_name;

    ui->inf_name->setText(detail.INF_NAME);
    switch(detail.RECEIVER){
    case 0:
        res_name = u8"全体师生:";
        break;
    case 1:
        res_name = u8"老师:";
        break;
    case 2:
        res_name = u8"学生:";
        break;
    }
    ui->receiver->setText(detail.GRADE+detail.MNAME+res_name);
    qDebug()<<detail.RECEIVER;
//    ui->content->setText("    "+detail.CONTENT);
//    ui->content->adjustSize();
//    ui->content->setWordWrap(true);
    QRegExp rx("\n");
    detail.CONTENT.replace(rx, "\n    ");
    ui->textBrowser->setText("    " + detail.CONTENT);
    ui->itime->setText(detail.ITIME);
}
