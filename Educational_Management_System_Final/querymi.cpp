#include "querymi.h"
#include "ui_querymi.h"
#include"update_progress_thread.h"
#include"query_allmi_thread.h"
queryMI::queryMI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::queryMI)
{
    ui->setupUi(this);

    connect(ui->lineEdit, SIGNAL(returnPressed()), ui->pushButton, SIGNAL(clicked()), Qt::UniqueConnection);
    ui->tableWidget->clear();
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);//给表格设置右键菜单
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//给表格设置不可编辑

    popMenu=new QMenu(ui->tableWidget);
    actDelete=new QAction("删除",this);
    actEdit=new QAction("修改",this);
    popMenu->addAction(actDelete);
    popMenu->addAction(actEdit);

    //初始化进度条
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);

    connect(actDelete,SIGNAL(triggered(bool)),this,SLOT(mi_slot_DL_ActDelete()));
    connect(actEdit,SIGNAL(triggered(bool)),this,SLOT(mi_slot_DL_ActEdit()));
}

queryMI::~queryMI()
{
    delete ui;
}
//更新进度条的槽函数，从进度条thread获取实时的进度值并更新进度条
void queryMI::progressValue2(int pvalue)
{
    if(pvalue<=100)
        ui->progressBar->setValue(pvalue);
}
void queryMI::on_pushButton_clicked()
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
    query_allmi_thread *qat=new query_allmi_thread(keywords,this);
    connect(qat,SIGNAL(resultReady(int)),this,SLOT(slot_show_result(int)));
    connect(qat,SIGNAL(finished()),qat,SLOT(deleteLater()));//线程结束后自动销毁
    qat->start();
    ui->pushButton->setDisabled(true);

}

void queryMI::slot_show_result(int errorCode){
    if(errorCode == 0){
        ui->tableWidget->clear();
        ui->tableWidget->setColumnCount(5);
        QStringList header;
        vector<MI>::iterator begin;
        vector<MI>::iterator end;
        header<<"专业名"<<"专业号"<<"学院名"<<"学院号"<<"办公地址";
        begin = miTableInfo.begin();
        end = miTableInfo.end();
        ui->tableWidget->setRowCount(miTableInfo.size());
        ui->tableWidget->setHorizontalHeaderLabels(header);
        int i = 0;
        for(vector<MI>::iterator it = begin;it!=end;it++){
            ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString(it->MNAME)));
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString(it->MNO)));
            ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString(it->INAME)));
            ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString(it->INO)));
            ui->tableWidget->setItem(i,4,new QTableWidgetItem(QString(it->IPLACE)));
            i++;
        }
        ui->num->setText(QString::number(i));
        ui->tableWidget->repaint();
    }
    else if(errorCode == 1)
        QMessageBox::information(NULL, "错误", "查询的时候发生错误，错误信息："+errorMessage);
    ui->pushButton->setEnabled(true);
}
//弹出菜单的槽函数
void queryMI::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    select_row=-1;
    QTableWidgetItem *selected_item=ui->tableWidget->itemAt(pos);
    if(selected_item!=NULL)
    {
        select_row=selected_item->row();
        selected_MNAME=ui->tableWidget->item(select_row,0)->text();
        popMenu->exec(QCursor::pos());
    }
}

void queryMI::mi_slot_DL_ActDelete(){
    int nRat = QMessageBox::question(NULL,"确定是否删除该专业","\n您要删除的专业是，"+selected_MNAME,
                                     QMessageBox::Yes, QMessageBox::No);
    //如果用户确定删除
    if (nRat == QMessageBox::Yes)
    {
        QSqlQuery query;
        query.clear();
        if(!query.exec("delete from MI where MNAME = '"+selected_MNAME.trimmed()+"'"))
        {
            errorMessage=query.lastError().text();
            QMessageBox::information(NULL, "错误", "删除的时候发生错误，错误信息："+errorMessage);
        }
        /*
        query.clear();
        query.prepare("delete from TIMETABLE where MNAME like ?");
        query.addBindValue(selected_MNAME);
        if(!query.exec())
        {
            errorMessage=query.lastError().text();
            QMessageBox::information(NULL, "错误", "删除的时候发生错误，错误信息："+errorMessage);
        }*/


        if(select_row!=-1)
            ui->tableWidget->removeRow(select_row);
        ui->tableWidget->repaint();
    }
}
void queryMI::mi_slot_DL_ActEdit(){
    QSqlQuery query;
    MI tmp;
    query.clear();
    if(!query.exec("select distinct * from MI where MNAME = '"+selected_MNAME.trimmed()+"'"))
    {
        errorMessage=query.lastError().text();
        QMessageBox::information(NULL,"错误！","在查询数据库时发生错误，错误信息："+errorMessage);
    }
    //create table MI(MNO char(3),MNAME char(230),INO char(3),INAME char(230),IPLACE varchar2(150),primary key(MNAME));
    else{
        while(query.next()){
            tmp.MNO = query.value(0).toString().trimmed();
            tmp.MNAME = query.value(1).toString().trimmed();
            tmp.INO = query.value(2).toString().trimmed();
            tmp.INAME = query.value(3).toString().trimmed();
            tmp.IPLACE = query.value(4).toString().trimmed();
        }
        EMD = new edit_mi_dialog(tmp);
        connect(EMD,&edit_mi_dialog::sendNewMI,this,&queryMI::mi_slot_update_table);
        EMD->show();
    }
}
void queryMI::mi_slot_update_table(MI after_edit){
    if(select_row != -1){
        ui->tableWidget->setItem(select_row,0,new QTableWidgetItem(after_edit.MNO));
        ui->tableWidget->setItem(select_row,2,new QTableWidgetItem(after_edit.INO));
        ui->tableWidget->setItem(select_row,3,new QTableWidgetItem(after_edit.INO));
        ui->tableWidget->setItem(select_row,4,new QTableWidgetItem(after_edit.IPLACE));
    }
    ui->tableWidget->repaint();
}

//导出为CSV文件
void queryMI::on_pushButton_2_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, "请选择存放CSV的路径", "./" + ui->lineEdit->text().trimmed()+"学院专业表查询结果", tr("CSV file (*.csv)"));
    if (!path.isEmpty())
    {
        if(outCSVfromTable(path,ui->tableWidget,"专业名,专业号,学院名,学院号,办公地址"))
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
