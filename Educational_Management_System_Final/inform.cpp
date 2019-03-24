#include "inform.h"
#include "ui_inform.h"

Inform::Inform(basic currtea,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Inform)
{
    ui->setupUi(this);

    ui->dateTimeEdit->setMinimumDate(QDate::currentDate().addDays(-365));  // -365天
    ui->dateTimeEdit->setMaximumDate(QDate::currentDate().addDays(365));  // +365天
    ui->dateTimeEdit->setCalendarPopup(true);

    //设置为当前时间
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    ui->tableWidget->clear();
    this->currTea=currtea;
}


void Inform::on_pushButton_clicked()
{

    ui->tableWidget->clear();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//给表格设置不可编辑

    //获取老师id
    QString tno=currTea.ID.trimmed();
    int errorcode=classnamelist(tno);
    if(errorcode==0)
    {
        ui->tableWidget->clear();
        ui->tableWidget->setColumnCount(5);
        QStringList header;//设置表头内容
        header<<"  "<<"年级"<<"班级"<<"课程号"<<"课程名";


        vector<teachinf3>::iterator begin1;//定义遍历器的开始
        vector<teachinf3>::iterator end1;//定义遍历器的结束


        //该老师所教课程的开始结束遍历器设置
        begin1=teachcourse3.begin();
        end1=teachcourse3.end();

        ui->tableWidget->setRowCount(teachcourse3.size());
        //qDebug()<<teachcourse3.size();
        ui->tableWidget->setHorizontalHeaderLabels(header);


        int i=0;
        //设置表格内容
        vector<teachinf3>::iterator it1=begin1;
        for(;it1!=end1;)
        {
            QTableWidgetItem *checkBox = new QTableWidgetItem();
            checkBox->setCheckState(Qt::Unchecked);
            ui->tableWidget->setItem(i,0,checkBox);
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(it1->GRADE));
            ui->tableWidget->setItem(i,2,new QTableWidgetItem(it1->MNAME));
            ui->tableWidget->setItem(i,3,new QTableWidgetItem(it1->CNO));
            ui->tableWidget->setItem(i,4,new QTableWidgetItem(it1->CNAME));
            i++;
            it1++;

        }
        ui->tableWidget->repaint();//让表格刷新显示

    }
    else if(errorcode==1)
        QMessageBox::information(NULL, "错误", "查询的时候发生错误，错误信息："+errorMessage);

}

void Inform::on_pushButton_2_clicked()
{
    select_row1=-1;
    teachinf5 tmp;


    tmp.INF_NAME=ui->textEdit_2->toPlainText();;
    tmp.CONTENT=ui->textEdit->toPlainText();;

    //获取当前用户选择的时间
    QDateTime dateTime = ui->dateTimeEdit->dateTime();


    tmp.ITIME=dateTime.toString("yyyy-MM-dd hh:mm:ss");
    //qDebug() << "DateTime : " <<tmp.ITIME;

    int i;
    int j=0;
    QSqlQuery query;
    for(i=0;i<teachcourse3.size();i++)
    {
        if(ui->tableWidget ->item(i, 0)->checkState() == Qt::Checked)
        {
            select_row1=i;
            tmp.GRADE=ui->tableWidget->item(select_row1,1)->text().trimmed();
            tmp.MNAME=ui->tableWidget->item(select_row1,2)->text().trimmed();

            query.clear();
            /*receiver
             * 0 全体
             * 1 老师
             * 2 学生*/
            query.prepare("insert into INFORM values(?,?,to_date(?,'YYYY-MM-DD HH24:MI:SS'),?,?,2)");
            query.addBindValue(tmp.INF_NAME);
            query.addBindValue(tmp.CONTENT);
            query.addBindValue(tmp.ITIME);
            query.addBindValue(tmp.MNAME);
            query.addBindValue(tmp.GRADE);
            bool pan=query.exec();
            if(!pan)
            {
                qDebug()<<query.lastError()<<endl;
                errorMessage=query.lastError().text();
                j=-1;
                QMessageBox::information(NULL,"错误！","在写入数据库时发生错误，错误信息："+errorMessage);
            }
            else
                j++;
        }

    }
    if(j>0)
    {
        QString p="";
        p=p+"成功发送"+QString::number(j, 10) +"条通知";
        QMessageBox::information(NULL,"OK",p);
    }
}

Inform::~Inform()
{
    delete ui;
}


