#include "query_all.h"
#include "ui_query_all.h"
#include"update_progress_thread.h"
#include"query_all_thread.h"

query_all::query_all(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::query_all)
{
    ui->setupUi(this);

    //绑定按下回车点击确定查询按钮
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

    connect(actDelete, SIGNAL(triggered()), this, SLOT(slot_DL_ActDelete()));
    connect(actEdit, SIGNAL(triggered()), this, SLOT(slot_DL_ActEdit()));

}

query_all::~query_all()
{
    delete ui;
}

//更新进度条的槽函数，从进度条thread获取实时的进度值并更新进度条
void query_all::progressValue2(int pvalue)
{
    if(pvalue<=100)
        ui->progressBar->setValue(pvalue);
}

//点击确认查询按钮
void query_all::on_pushButton_clicked()
{
    ui->tableWidget->clear();
    int userType=ui->comboBox->currentIndex()+1;//先确定用户选择的用户类型号码
    QString keywords="";
    //获取用户输入的关键字
    keywords=ui->lineEdit->text();

    progress=0;//首先初始化进度条为0
    //创建一个线程用于更新进度条
    Update_Progress_Thread *UPT=new Update_Progress_Thread(this);
    connect(UPT,SIGNAL(resultReady(int)),this,SLOT(progressValue2(int)));
    connect(UPT,SIGNAL(finished()),UPT,SLOT(deleteLater()));//线程结束后自动销毁
    UPT->start();

    //创建一个线程用于后台导入数据库
    query_all_thread *qat=new query_all_thread(userType,keywords,this);
    connect(qat,SIGNAL(resultReady(int,int)),this,SLOT(slot_show_result(int,int)));
    connect(qat,SIGNAL(finished()),qat,SLOT(deleteLater()));//线程结束后自动销毁
    qat->start();
    ui->pushButton->setDisabled(true);

}


//在执行完查询后把结果放到表格显示给用户看
void query_all::slot_show_result(int errorCode,int userType)
{

    if(errorCode==0)
    {
        ui->tableWidget->clear();//每次使用表格前先清除上次留下来的内容
        ui->tableWidget->setColumnCount(6); //设置列数

        //如果查询成功，则展示结果到表格上面
        QStringList header;//设置表头内容
        vector<basic>::iterator begin;//定义遍历器的开始
        vector<basic>::iterator end;//定义遍历器的结束
        switch(userType)
        {
        //学生的表头和开始结束遍历器设置
        case 1:
            header<<"学号"<<"姓名"<<"性别"<<"身份证号码"<<"专业班级"<<"联系方式";
            begin=studentInfo.begin();
            end=studentInfo.end();
            ui->tableWidget->setRowCount(studentInfo.size());//设置行数
            break;
            //老师的表头和开始结束遍历器设置
        case 2:
            header<<"工号"<<"姓名"<<"性别"<<"身份证号码"<<"所在学院"<<"联系方式";
            begin=teacherInfo.begin();
            end=teacherInfo.end();
            ui->tableWidget->setRowCount(teacherInfo.size());//设置行数
            break;
            //管理员的表头和开始结束遍历器设置
        case 3:
            header<<"工号"<<"姓名"<<"性别"<<"身份证号码"<<"所在学院"<<"联系方式";
            begin=adminInfo.begin();
            end=adminInfo.end();
            ui->tableWidget->setRowCount(adminInfo.size());//设置行数
            break;
        }
        ui->tableWidget->setHorizontalHeaderLabels(header);
        int i=0;
        //设置表格内容
        for(vector<basic>::iterator it=begin;it!=end;it++)
        {
            ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString(it->ID)));
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString(it->name)));
            ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString(it->sex)));
            //ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString(it->password)));
            ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString(it->IDnumber)));
            ui->tableWidget->setItem(i,4,new QTableWidgetItem(QString(it->label)));
            ui->tableWidget->setItem(i,5,new QTableWidgetItem(QString(it->tel)));
            i++;
        }

        ui->numl->setText(QString::number(i));
        ui->tableWidget->repaint();//让表格刷新显示
    }
    else if(errorCode==1)
        QMessageBox::information(NULL, "错误", "查询的时候发生错误，错误信息："+errorMessage);
    ui->pushButton->setEnabled(true);
}


//弹出菜单的槽函数
void query_all::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    select_row=-1;
    QTableWidgetItem *selected_item=ui->tableWidget->itemAt(pos);
    if(selected_item!=NULL)
    {
        select_row=selected_item->row();
        selected_NO=ui->tableWidget->item(select_row,0)->text();//获取当前选中对象的学/工号，方便在数据库中找到对应记录进行删除
        popMenu->exec(QCursor::pos());
    }
}

//右键菜单里面的删除按钮
void query_all::slot_DL_ActDelete()
{
    int nRat = QMessageBox::question(NULL,"确定是否删除该用户","\n您要删除的用户的ID是，"+selected_NO.trimmed(),
                                     QMessageBox::Yes, QMessageBox::No);
    //如果用户确定删除该用户
    if (nRat == QMessageBox::Yes)
    {
        QSqlQuery query;
        //此处执行删除某个用户的操作
        //首先判断是删除哪种用户
        switch(ui->comboBox->currentIndex())
        {
        //需要删除一个学生
        case 0:
            query.clear();
            //因为有外码约束，所以删除一个学生，要在SC表，GPA表里面把对应着该学生的信息一并删除

            if(!query.exec("delete from SC where SNO  = '"+selected_NO.trimmed()+"'"))
            {
                errorMessage=query.lastError().text();
                QMessageBox::information(NULL, "错误", "删除的时候发生错误，错误信息："+errorMessage);
            }

            query.clear();
            //因为有外码约束，所以删除一个学生，要在SC表，GPA表里面把对应着该学生的信息一并删除
            if(!query.exec("delete from GPA where SNO  = '"+selected_NO.trimmed()+"'"))
            {
                errorMessage=query.lastError().text();
                QMessageBox::information(NULL, "错误", "删除的时候发生错误，错误信息："+errorMessage);
            }

            query.clear();
            //因为有外码约束，所以删除一个学生，要在SC表，GPA表里面把对应着该学生的信息一并删除，最后再删STUDENT表里面的记录
            if(!query.exec("delete from STUDENT where SNO  = '"+selected_NO.trimmed()+"'"))
            {
                errorMessage=query.lastError().text();
                QMessageBox::information(NULL, "错误", "删除的时候发生错误，错误信息："+errorMessage);
            }
            //把删除的学号放到回收站
            query.clear();
            query.prepare("insert into RECYCLE values(?)");
            query.addBindValue(selected_NO.trimmed());
            if(!query.exec())
            {
                errorMessage=query.lastError().text();
                QMessageBox::information(NULL, "错误", "插入的时候发生错误，错误信息："+errorMessage);
            }

            break;
            //需要删除一个老师
        case 1:


            //删除一个老师要记得在TIMETABLE和TEACHER把对应的记录删除
            query.clear();

            if(!query.exec("delete from TIMETABLE where TNO = '"+selected_NO.trimmed()+"'"))
            {
                errorMessage=query.lastError().text();
                QMessageBox::information(NULL, "错误", "删除的时候发生错误，错误信息："+errorMessage);
            }

            //删除一个老师要记得在TIMETABLE和TEACHER把对应的记录删除
            query.clear();
            if(!query.exec("delete from TEACHER where TNO  =  '"+selected_NO.trimmed()+"'"))
            {
                errorMessage=query.lastError().text();
                QMessageBox::information(NULL, "错误", "删除的时候发生错误，错误信息："+errorMessage);
            }

            //把删除的学号放到回收站
            query.clear();
            query.prepare("insert into RECYCLE values(?)");
            query.addBindValue(selected_NO.trimmed());
            if(!query.exec())
            {
                errorMessage=query.lastError().text();
                QMessageBox::information(NULL, "错误", "插入的时候发生错误，错误信息："+errorMessage);
            }

            break;
            //需要删除一个管理员
        case 2:
            query.clear();
            if(!query.exec("delete from ADMIN where ANO = '"+selected_NO.trimmed()+"'"))
            {
                errorMessage=query.lastError().text();
                QMessageBox::information(NULL, "错误", "删除的时候发生错误，错误信息："+errorMessage);
            }

            //把删除的学号放到回收站
            query.clear();
            query.prepare("insert into RECYCLE values(?)");
            query.addBindValue(selected_NO.trimmed());
            if(!query.exec())
            {
                errorMessage=query.lastError().text();
                QMessageBox::information(NULL, "错误", "插入的时候发生错误，错误信息："+errorMessage);
            }


            break;
        }
        //处理表格显示的问题，把表格中的这条记录删除，然后刷新显示
        if(select_row!=-1)
            ui->tableWidget->removeRow(select_row);
        ui->tableWidget->repaint();
    }
}

//右键菜单的编辑按钮
void query_all::slot_DL_ActEdit()
{
    basic tmp;
    QSqlQuery query;

    switch(ui->comboBox->currentIndex())
    {
    //学生类型
    case 0:
        query.clear();
        if(!query.exec("select distinct * from STUDENT where SNO = '"+selected_NO.trimmed()+"'"))
        {
            errorMessage=query.lastError().text();
            QMessageBox::information(NULL,"错误！","在查询数据库时发生错误，错误信息："+errorMessage);
        }
        else
        {
            while(query.next())
            {
                tmp.ID=query.value(0).toString().trimmed();
                tmp.name=query.value(1).toString().trimmed();
                tmp.sex=query.value(2).toString().trimmed();
                tmp.password=query.value(3).toString().trimmed();
                tmp.IDnumber=query.value(4).toString().trimmed();
                tmp.label=query.value(5).toString().trimmed();
                tmp.tel=query.value(6).toString().trimmed();
            }
            EUD=new Edit_User_Dialog(0,tmp);
            connect(EUD, &Edit_User_Dialog::sendNewUser, this, &query_all::slot_update_table);
            EUD->show();
            //delete EUD;

        }
        break;
        //老师类型
    case 1:
        query.clear();
        if(!query.exec("select distinct * from TEACHER where TNO = '"+selected_NO.trimmed()+"'"))
        {
            errorMessage=query.lastError().text();
            QMessageBox::information(NULL,"错误！","在查询数据库时发生错误，错误信息："+errorMessage);
        }
        else
        {
            while(query.next())
            {
                tmp.ID=query.value(0).toString().trimmed();
                tmp.name=query.value(1).toString().trimmed();
                tmp.sex=query.value(2).toString().trimmed();
                tmp.password=query.value(3).toString().trimmed();
                tmp.IDnumber=query.value(4).toString().trimmed();
                tmp.label=query.value(5).toString().trimmed();
                tmp.tel=query.value(6).toString().trimmed();
            }
            EUD=new Edit_User_Dialog(0,tmp);
            connect(EUD, &Edit_User_Dialog::sendNewUser, this, &query_all::slot_update_table);
            EUD->show();
        }
        break;
        //管理员类型
    case 2:
        query.clear();
        if(!query.exec("select distinct * from ADMIN where ANO = '"+selected_NO.trimmed()+"'"))
        {
            errorMessage=query.lastError().text();
            QMessageBox::information(NULL,"错误！","在查询数据库时发生错误，错误信息："+errorMessage);
        }
        else
        {
            while(query.next())
            {
                tmp.ID=query.value(0).toString().trimmed();
                tmp.name=query.value(1).toString().trimmed();
                tmp.sex=query.value(2).toString().trimmed();
                tmp.password=query.value(3).toString().trimmed();
                tmp.IDnumber=query.value(4).toString().trimmed();
                tmp.label=query.value(5).toString().trimmed();
                tmp.tel=query.value(6).toString().trimmed();
            }
            EUD=new Edit_User_Dialog(0,tmp);
            connect(EUD, &Edit_User_Dialog::sendNewUser, this, &query_all::slot_update_table);
            EUD->show();
        }
        break;
    }
}
void query_all::slot_update_table(basic after_edit)
{
    if(select_row!=-1)
    {
        ui->tableWidget->setItem(select_row,1,new QTableWidgetItem(after_edit.name));

        ui->tableWidget->setItem(select_row,2,new QTableWidgetItem(after_edit.sex));

        ui->tableWidget->setItem(select_row,3,new QTableWidgetItem(after_edit.IDnumber));
        ui->tableWidget->setItem(select_row,4,new QTableWidgetItem(after_edit.label));
        ui->tableWidget->setItem(select_row,5,new QTableWidgetItem(after_edit.tel));
    }
    ui->tableWidget->repaint();
}

//导出查询结果为csv文件
void query_all::on_pushButton_2_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, "请选择存放CSV的路径", "./" + ui->comboBox->currentText()+"列表查询结果", tr("CSV file (*.csv)"));
   if (!path.isEmpty())
    {
        if(outCSVfromTable(path,ui->tableWidget,"ID号,姓名,性别,身份证号码,学院或专业,联系方式"))
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
