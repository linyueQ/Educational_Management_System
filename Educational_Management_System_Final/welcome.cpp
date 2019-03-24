#include "welcome.h"
#include "ui_welcome.h"
#include <QHeaderView>
Welcome::Welcome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Welcome)
{
    ui->setupUi(this);

}

Welcome::~Welcome()
{
    delete ui;
}
Welcome::Welcome(int userType ,basic currentUser,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Welcome)
{
    ui->setupUi(this);
    this->currentUser = currentUser;
    this->userType=userType;
    this->init();
    connect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(on_cilck_table(QTableWidgetItem *)));
}
//初始化通知列表函数
void Welcome::init()
{
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setFrameShape(QFrame::NoFrame);
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList header;
    header<<"通知栏";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setStyleSheet("selection-background-color:lightblue;");
    ui->tableWidget->setColumnWidth(0,800);
    QSqlQuery query;
    int sum;
    query.clear();
    /*receiver
     * 0 全体
     * 1 老师
     * 2 学生*/
    int i=0;
    switch(userType){
    case 0:
    {

        query.exec("select MNAME from STUDENT where SNO = '"+this->currentUser.ID+"'");
        QString mname ="";
        if(query.next())
        {
            mname = query.value(0).toString().trimmed();
        }

        query.clear();
        if(query.exec("select INF_NAME from INFORM where MNAME ='"+mname+"' and RECEIVER = 2 and GRADE='"+currentUser.ID.left(4)+"' or receiver=0"))
        {
            query.last();
            sum = query.at()+1;
            query.first();
            query.previous();
            ui->tableWidget->setRowCount(sum);
            while(query.next())
            {
                ui->tableWidget->setItem(i,0,new QTableWidgetItem(query.value(0).value<QString>().trimmed()));
                i++;
            }
        }

        break;
    }
    case 1:
    {
        /*receiver
         * 0 全体
         * 1 老师
         * 2 学生*/
        query.exec("select INF_NAME from INFORM where RECEIVER =1 or receiver=0");
        query.last();
        sum = query.at()+1;
        query.first();
        query.previous();
        ui->tableWidget->setRowCount(sum);
        while(query.next()){
            ui->tableWidget->setItem(i,0,new QTableWidgetItem(query.value(0).value<QString>().trimmed()));
            i++;
        }

        break;
    }
    case 2:
    {
        query.exec("select INF_NAME from INFORM ");

        //ui->tableWidget->setRowCount(query.size()==-1?0:query.size());
        query.last();
        sum = query.at()+1;
        query.first();
        query.previous();
        ui->tableWidget->setRowCount(sum);
        while(query.next())
        {
            ui->tableWidget->setItem(i,0,new QTableWidgetItem(query.value(0).value<QString>().trimmed()));
            i++;
        }

        break;
    }
    }
}

//点击了表格项需要执行的函数
void Welcome::on_cilck_table(QTableWidgetItem *item)
{
    QString clickInfName=item->text().trimmed();
    QSqlQuery query ;
    query.clear();
    query.exec("select * from INFORM where INF_NAME ='"+clickInfName +"'");
    if(!query.exec())
    {
        errorMessage=query.lastError().text();
        QMessageBox::information(NULL,"错误！","在更新数据库时发生错误，错误信息："+errorMessage);
    }
    while(query.next())
    {
        detail.INF_NAME = query.value(0).toString().trimmed();
        detail.CONTENT = query.value(1).toString().trimmed();
        detail.ITIME = query.value(2).value<QDateTime>().toString("yyyy年MM月dd日 hh:mm:ss");
        detail.MNAME = query.value(3).toString().trimmed();
        detail.GRADE = query.value(4).toString().trimmed();
        detail.RECEIVER = query.value(5).toInt();
    }
    //    emit sendDetail(detail);
    IDD = new informDetailDialog(detail);
    //connect(this,&queryInform::sendDetail,IDD,&informDetailDialog);
    IDD->show();
}

//刷新通知表按钮
void Welcome::on_pushButton_clicked()
{
    this->init();
}
