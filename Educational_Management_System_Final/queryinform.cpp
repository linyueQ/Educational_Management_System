#include "queryinform.h"
#include "ui_queryinform.h"
#include"update_progress_thread.h"
#include"query_allinform_thread.h"
queryInform::queryInform(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::queryInform)
{
    ui->setupUi(this);

    connect(ui->lineEdit, SIGNAL(returnPressed()), ui->pushButton, SIGNAL(clicked()), Qt::UniqueConnection);
    ui->tableWidget->clear();
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);//给表格设置右键菜单
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//给表格设置不可编辑

    popMenu=new QMenu(ui->tableWidget);
    actDelete=new QAction("删除",this);
    popMenu->addAction(actDelete);
    actDetail = new QAction("Detail",this);
    popMenu->addAction(actDetail);

    //初始化进度条
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);
    connect(actDelete,SIGNAL(triggered(bool)),this,SLOT(in_slot_DL_ActDelete()));
    connect(actDetail,SIGNAL(triggered(bool)),this,SLOT(in_slot_DL_ActDetail()));
}

queryInform::~queryInform()
{
    delete ui;
}

void queryInform::progressValue2(int pvalue){
    {
        if(pvalue<=100)
            ui->progressBar->setValue(pvalue);
    }
}


void queryInform::on_pushButton_clicked()
{
    ui->tableWidget->clear();
    QString keywords = "";
    keywords = ui->lineEdit->text();
    progress = 0;

    Update_Progress_Thread *UPT=new Update_Progress_Thread(this);
    connect(UPT,SIGNAL(resultReady(int)),this,SLOT(progressValue2(int)));
    connect(UPT,SIGNAL(finished()),UPT,SLOT(deleteLater()));//线程结束后自动销毁
    UPT->start();

    //创建一个线程用于后台导入数据库
    query_allinform_thread *qat=new query_allinform_thread(keywords,this);
    connect(qat,SIGNAL(resultReady(int)),this,SLOT(slot_show_result(int)));
    connect(qat,SIGNAL(finished()),qat,SLOT(deleteLater()));//线程结束后自动销毁
    qat->start();
    ui->pushButton->setDisabled(true);
}

void queryInform::slot_show_result(int errorCode){
    if(errorCode == 0){
        ui->tableWidget->clear();
        ui->tableWidget->setColumnCount(6);
        QStringList header;
        vector<inform>::iterator begin;
        vector<inform>::iterator end;
        ui->tableWidget->setColumnWidth(1,50);
        ui->tableWidget->setColumnWidth(2,180);
        ui->tableWidget->setColumnWidth(3,250);
        ui->tableWidget->setColumnWidth(5,55);
        header<<"通知名"<<"通知对象"<<"发布时间"<<"内容"<<"专业"<<"年级";
        begin = informTableInfo.begin();
        end = informTableInfo.end();
        ui->tableWidget->setRowCount(informTableInfo.size());
        ui->tableWidget->setHorizontalHeaderLabels(header);
        int i = 0;
        for(vector<inform>::iterator it = begin;it!=end;it++){
            ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString(it->INF_NAME)));
            QString name;
            switch(it->RECEIVER){
            case 0:
                ui->tableWidget->setItem(i,1,new QTableWidgetItem(tr("全体")));break;
            case 1:
                ui->tableWidget->setItem(i,1,new QTableWidgetItem(tr("老师")));break;
            case 2:
                ui->tableWidget->setItem(i,1,new QTableWidgetItem(tr("学生 ")));break;
            }

            ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString(it->ITIME)));

            ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString(it->CONTENT)));
            ui->tableWidget->setItem(i,4,new QTableWidgetItem(QString(it->MNAME)));
            ui->tableWidget->setItem(i,5,new QTableWidgetItem(QString(it->GRADE)));
            i++;
        }
        ui->numl->setText(QString::number(i));
        ui->tableWidget->repaint();
    }
    else if(errorCode == 1)
        QMessageBox::information(NULL, "错误", "查询的时候发生错误，错误信息："+errorMessage);
    ui->pushButton->setEnabled(true);
}

//弹出菜单的槽函数
void queryInform::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    select_row=-1;
    QTableWidgetItem *selected_item=ui->tableWidget->itemAt(pos);
    if(selected_item!=NULL)
    {
        select_row=selected_item->row();
        selected_INFORM=ui->tableWidget->item(select_row,0)->text();
        popMenu->exec(QCursor::pos());
    }
}

void queryInform::in_slot_DL_ActDelete(){
    int nRat = QMessageBox::question(NULL,"确定是否删除该通知","\n您要删除的通知是，"+selected_INFORM,
                                     QMessageBox::Yes, QMessageBox::No);
    //如果用户确定删除
    if (nRat == QMessageBox::Yes)
    {
        QSqlQuery query;
        query.clear();
        if(!query.exec("delete from INFORM where INF_NAME = '"+selected_INFORM.trimmed()+"'"))
        {
            errorMessage=query.lastError().text();
            QMessageBox::information(NULL, "错误", "删除的时候发生错误，错误信息："+errorMessage);
        }

        if(select_row!=-1)
            ui->tableWidget->removeRow(select_row);
        ui->tableWidget->repaint();
    }
}
void queryInform::in_slot_DL_ActDetail(){
    QSqlQuery query ;
    query.clear();
    query.exec("select * from INFORM where INF_NAME ='"+selected_INFORM +"'");
    if(!query.exec())
    {
        errorMessage=query.lastError().text();
        QMessageBox::information(NULL,"错误！","在更新数据库时发生错误，错误信息："+errorMessage);
    }
    while(query.next()){
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

//导出查询结果为CSv文件
void queryInform::on_pushButton_2_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, "请选择存放CSV的路径", "./" + ui->lineEdit->text().trimmed()+"通知表查询结果", tr("CSV file (*.csv)"));
    if (!path.isEmpty())
    {
        if(outCSVfromTable(path,ui->tableWidget,"通知名,通知对象,发布时间,通知内容,专业,年级"))
        {
            QMessageBox::information(NULL, "成功！", "结果已经保存到以下文件 "+path);
        }
        else
        {
            QMessageBox::information(NULL, "失败！", "在以下路径无法创建文件 "+path);
        }
    }
    else
    {
        QMessageBox::information(NULL, "错误！", "你未选择保存到哪个路径！");
    }
}
