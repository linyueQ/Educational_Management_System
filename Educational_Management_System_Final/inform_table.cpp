#include "inform_table.h"
#include "ui_inform_table.h"
#include "base.h"
inform_Table::inform_Table(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inform_Table)
{
    ui->setupUi(this);
    ui->inameType->setEnabled(false);
    ui->mnameType->setEnabled(false);
    ui->GRADE->setEnabled(false);

}

inform_Table::~inform_Table()
{
    delete ui;
}

void inform_Table::on_userType_currentIndexChanged(int index)
{
    int userType = ui->userType->currentIndex();
    QSqlQuery query;
    switch(userType){
    case 0:
        ui->inameType->setEnabled(false);
        ui->mnameType->setEnabled(false);
        ui->GRADE->setEnabled(false);
        query.clear();
        break;
    case 1:
        ui->inameType->setEnabled(true);
        ui->mnameType->setEnabled(false);
        ui->GRADE->setEnabled(false);
        ui->inameType->clear();
        query.clear();
        query.exec("select distinct INAME from MI");
        while(query.next()){
            ui->inameType->addItem(query.value(0).toString().trimmed());
        }
        break;
    case 2:
        ui->mnameType->setEnabled(true);
        ui->inameType->setEnabled(true);
        ui->GRADE->setEnabled(true);
        ui->inameType->clear();

        query.clear();
        query.exec("select distinct INAME from MI");
        while(query.next()){
            ui->inameType->addItem(query.value(0).toString().trimmed());
        }
        break;
    }
}
//create table INFORM(INF_NAME char(230),CONTENT varchar2(2000),to_date(?,'YYYY-MM-DD HH24:MI:SS'),RECEIVER int,primary key(INF_NAME));
//"insert into INFORM values(?,?,to_date(?,'YYYY-MM-DD HH24:MI:SS'),?,?,?)"
//create table INFORM(INF_NAME char(230),CONTENT varchar2(2000),ITIME date,MNAME char(230),GRADE char(4),RECEIVER int,primary key(GRADE,MNAME,INF_NAME));

void inform_Table::on_pushButton_okin_clicked()
{
    inform tmp;
    QSqlQuery query;
    tmp.INF_NAME = ui->informName->text();
    tmp.CONTENT = QString(ui->context->toPlainText());
    tmp.RECEIVER = ui->userType->currentIndex();
    /*receiver
     * 0 全体
     * 1 老师
     * 2 学生*/

    tmp.GRADE = ui->GRADE->currentText();
    tmp.MNAME = ui->mnameType->currentText();

    // 获取当前的系统时间
    QDateTime dateTime = QDateTime::currentDateTime();
    tmp.ITIME = dateTime.toString("yyyy-MM-dd hh:mm:ss");
    //qDebug()<<tmp.ITIME;


    int nRat = QMessageBox::question(NULL,"确定是否添加该信息","是否发布该通知：\n"+tmp.INF_NAME,QMessageBox::Yes,QMessageBox::No);
    if(nRat == QMessageBox::Yes){
        query.clear();
        switch(tmp.RECEIVER) {
        case 2:
            query.prepare("insert into INFORM values(?,?,to_date(?,'YYYY-MM-DD HH24:MI:SS'),?,?,?)");;
            query.addBindValue(tmp.INF_NAME);
            query.addBindValue(tmp.CONTENT);
            query.addBindValue(tmp.ITIME);
            query.addBindValue(tmp.MNAME);
            query.addBindValue(tmp.GRADE);
            query.addBindValue(tmp.RECEIVER);
            break;
        default:
            query.prepare("insert into INFORM values(?,?,to_date(?,'YYYY-MM-DD HH24:MI:SS'),' ',' ',?)");;
            query.addBindValue(tmp.INF_NAME);
            query.addBindValue(tmp.CONTENT);
            query.addBindValue(tmp.ITIME);
            query.addBindValue(tmp.RECEIVER);
            break;
        }

        bool pan = query.exec();
        if(!pan){
            errorMessage=query.lastError().text();
            QMessageBox::information(NULL,"错误！","在写入数据库时发生错误，错误信息："+errorMessage);
        }
        else
            QMessageBox::information(NULL,"OK","成功写入!");
    }
}

void inform_Table::on_inameType_currentTextChanged(const QString &arg1)
{
    QSqlQuery query;
    if(ui->userType->currentIndex()==2){
        QString methon = "select distinct MNAME from MI where INAME = '";
        ui->mnameType->clear();
        methon.append(QString(ui->inameType->currentText())+"'");
        query.exec(methon);
        while(query.next()){
            ui->mnameType->addItem(query.value(0).toString().trimmed());
        }
    }
}
