#include "addonemitable.h"
#include "ui_addonemitable.h"
#include "base.h"
#include <QStringList>
#include <QList>
#include<QMessageBox>

//需要实现的表格
//create table MI(MNO char(3),MNAME char(230),INO char(3),INAME char(230),IPLACE varchar2(150),primary key(MNAME,INAME));
//MI（MNO（专业号）,MNAME(专业名)，INO（学院号），INAME（学院名），IPLACE（办公地址）)
//添加单个学院的单条记录
addOneMITable::addOneMITable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addOneMITable)
{
    ui->setupUi(this);
    init();
}
void addOneMITable::init(){
    QStringList inamelist;
    ui->comboBox->setEditable(true);
    QSqlQuery query;
    query.exec("select distinct INAME from MI");
    inamelist<<"";

    while(query.next()){
        inamelist.append(query.value(0).toString().trimmed());
    }

    ui->comboBox->addItems(inamelist);
}

addOneMITable::~addOneMITable()
{
    delete ui;
}

void addOneMITable::on_pushButton_clicked()
{


    MI tmp;
    QSqlQuery query;
    tmp.MNAME = ui->MNAME->text();
    tmp.INAME= ui->comboBox ->currentText();
    tmp.IPLACE = ui->IPlace->text();
    int nRat;
    if(ui->comboBox->currentIndex() == 0){
        query.clear();
        query.exec("select count(distinct ino) from mi");
        query.first();
        int inonum = query.value(0).value<int>();
        inonum ++;
        tmp.INO = "";
        tmp.INO += inonum / 10 + '0';
        tmp.INO += inonum % 10 + '0';
    }
    else {
        query.clear();
        query.exec("select ino from mi where iname = '"+ui->comboBox->currentText()+"'");
        while(query.next()){
            tmp.INO = query.value(0).toString().trimmed();
        }
    }
    query.clear();
    query.exec("select count(mno) from mi");
    query.first();
    int mount = query.value(0).value<int>();
    mount++;
    tmp.MNO = "";
    tmp.MNO += mount / 100 + '0';
    mount %= 100;
    tmp.MNO += mount / 10 + '0';
    tmp.MNO += mount % 10 + '0';
    nRat = QMessageBox::question(NULL,"确定是否添加该信息","\n您要添加的【学院/专业】的信息如下：\n专业号："+tmp.MNO+"\n专业名："+tmp.MNAME+"\n学院号："+tmp.INO+"\n学院名："+tmp.INAME+"\n办公地址："+tmp.IPLACE,QMessageBox::Yes, QMessageBox::No);
    if(nRat == QMessageBox::Yes){
        //此处添加一个信息的记录
        query.clear();
        query.prepare("insert into MI values(?,?,?,?,?)");
        query.addBindValue(tmp.MNO);
        query.addBindValue(tmp.MNAME);
        query.addBindValue(tmp.INO);
        query.addBindValue(tmp.INAME);
        query.addBindValue(tmp.IPLACE);
        qDebug()<<tmp.INO<<tmp.MNO;
        bool pan = query.exec();
        if(!pan)
        {
            qDebug()<<query.lastError()<<endl;
            errorMessage=query.lastError().text();
            QMessageBox::information(NULL,"错误！","在写入数据库时发生错误，错误信息："+errorMessage);
        }
        else
            QMessageBox::information(NULL,"OK","成功写入!");
    }

}

void addOneMITable::on_comboBox_currentTextChanged(const QString &arg1)
{
    if(ui->comboBox->currentIndex() > 0){
        ui->comboBox->setEditable(false);
    }
    else
        ui->comboBox->setEditable(true);
    QString name = ui->comboBox->currentText();
    QString methon="select ino,iplace from mi where iname =  ";
    QString inotext,iplacetext;
    methon.append("'"+name.trimmed()+ "'");
    QSqlQuery query;
    query.exec(methon);
    QSqlRecord rec = query.record();
    // 移动到第一条语句，如果移动成功则输出此条记录
    if(query.first())
    {
        rec = query.record();
        int inocol = rec.indexOf("ino");
        inotext = query.value(inocol).toString();
        int iplacecol = rec.indexOf("iplace");
        iplacetext = query.value(iplacecol).toString();
    }

    ui->IPlace->setText(iplacetext);
    ui->IPlace->show();
}
