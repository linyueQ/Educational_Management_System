#include "add_all.h"
#include "ui_add_all.h"

add_all::add_all(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_all)
{
    ui->setupUi(this);
//    this->setMinimumSize(1100,800);
//    this->setMaximumSize(1100,800);
    //新建ui的时候文件路径框的提示
    ui->lineEdit->setText("请点击右边的按钮选择文件导入...");
    ui->tableWidget->clear();

    ui->pushButton_2->setDisabled(true);

    //初始化进度条
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);
}

add_all::~add_all()
{
    delete ui;
}

//更新进度条的槽函数，从进度条thread获取实时的进度值并更新进度条
void add_all::progressValue(int pvalue)
{
    if(pvalue<=100)
        ui->progressBar->setValue(pvalue);
}

//写入数据库完成后的槽函数，从写入数据库的thread那里获取错误信息（插入失败的记录条数）
void add_all::errorSum(int errorsum)
{
    if(errorsum==0)
    {
        ui->tableWidget->clear();
        studentInfo.clear();
        teacherInfo.clear();
        adminInfo.clear();
        QMessageBox::information(this,"OK","所有用户的信息已经成功插入数据库!");
    }
    else
    {
        //否则这里进行出错处理，需要把插入错误的信息展示到表格上让用户重新修改这部分内容再尝试插入
        ui->lineEdit->setText("以下用户在导入的时候发生导入错误，请查看原因并尝试修正后重新导入！");

        ui->tableWidget->clear();//每次使用表格前先清除上次留下来的内容
        ui->tableWidget->setColumnCount(8); //设置列数

        //把插入失败的数据展示到表格上面，第一列展示失败原因
        QStringList header;//设置表头内容
        vector<basic>::iterator begin;//定义遍历器的开始
        vector<basic>::iterator end;//定义遍历器的结束

        header<<"插入失败原因"<<"学(工)号"<<"姓名"<<"性别"<<"密码"<<"身份证号码"<<"专业班级"<<"联系方式";
        begin=failInfo.begin();
        end=failInfo.end();
        ui->tableWidget->setRowCount(failInfo.size());//设置行数

        ui->tableWidget->setHorizontalHeaderLabels(header);
        int i=0;
        //设置表格内容
        for(vector<basic>::iterator it=begin;it!=end;it++)
        {
            ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString(it->errorMessage)));
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString(it->ID)));
            ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString(it->name)));
            ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString(it->sex)));
            ui->tableWidget->setItem(i,4,new QTableWidgetItem(QString(it->password)));
            ui->tableWidget->setItem(i,5,new QTableWidgetItem(QString(it->IDnumber)));
            ui->tableWidget->setItem(i,6,new QTableWidgetItem(QString(it->label)));
            ui->tableWidget->setItem(i,7,new QTableWidgetItem(QString(it->tel)));
            i++;
        }
        ui->tableWidget->repaint();//让表格刷新显示

    }
    ui->pushButton_2->setDisabled(true);
}

//确认导入按钮
void add_all::on_pushButton_2_clicked()
{
    progress=0;//首先初始化进度条为0
    //创建一个线程用于更新进度条
    Update_Progress_Thread *UPT=new Update_Progress_Thread(this);
    connect(UPT,SIGNAL(resultReady(int)),this,SLOT(progressValue(int)));
    connect(UPT,SIGNAL(finished()),UPT,SLOT(deleteLater()));//线程结束后自动销毁
    UPT->start();

    //创建一个线程用于后台导入数据库
    Import_Data_Thread *IDT=new Import_Data_Thread(ui->comboBox->currentIndex()+1,this);
    connect(IDT,SIGNAL(resultReady(int)),this,SLOT(errorSum(int)));
    connect(IDT,SIGNAL(finished()),IDT,SLOT(deleteLater()));//线程结束后自动销毁
    IDT->start();

}

//在结束读取csv文件的时候
void add_all::slot_after_readCSV(int errorCode,QString path,int type)
{
    if(errorCode)
    {
        progress=0;
        ui->lineEdit->setText(path);
        ui->tableWidget->clear();//每次使用表格前先清除上次留下来的内容
        ui->tableWidget->setColumnCount(7); //设置列数
        int sum=0;
        //如果数据导入成功，则展示到表格上面
        QStringList header;//设置表头内容
        vector<basic>::iterator begin;//定义遍历器的开始
        vector<basic>::iterator end;//定义遍历器的结束
        switch(type)
        {
        //学生的表头和开始结束遍历器设置
        case 1:
            header<<"学号"<<"姓名"<<"性别"<<"密码"<<"身份证号码"<<"专业班级"<<"联系方式";
            begin=studentInfo.begin();
            end=studentInfo.end();
            ui->tableWidget->setRowCount(studentInfo.size());//设置行数
            sum=studentInfo.size();
            break;
            //老师的表头和开始结束遍历器设置
        case 2:
            header<<"工号"<<"姓名"<<"性别"<<"密码"<<"身份证号码"<<"所在学院"<<"联系方式";
            begin=teacherInfo.begin();
            end=teacherInfo.end();
            ui->tableWidget->setRowCount(teacherInfo.size());//设置行数
            sum=teacherInfo.size();
            break;
            //管理员的表头和开始结束遍历器设置
        case 3:
            header<<"工号"<<"姓名"<<"性别"<<"密码"<<"身份证号码"<<"所在学院"<<"联系方式";
            begin=adminInfo.begin();
            end=adminInfo.end();
            ui->tableWidget->setRowCount(adminInfo.size());//设置行数
            sum=adminInfo.size();
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
            ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString(it->password)));
            ui->tableWidget->setItem(i,4,new QTableWidgetItem(QString(it->IDnumber)));
            ui->tableWidget->setItem(i,5,new QTableWidgetItem(QString(it->label)));
            ui->tableWidget->setItem(i,6,new QTableWidgetItem(QString(it->tel)));
            i++;
        }
        ui->tableWidget->repaint();//让表格刷新显示
        ui->pushButton_2->setEnabled(true);
    }
    else
    {
        QMessageBox::information(NULL, "错误", "在导入如下文件的时候发生错误！无法打开文件"+path);
        ui->lineEdit->setText("请点击右边的按钮选择文件导入...");
    }
}

//批量添加用户的 打开文件 按钮
void add_all::on_pushButton_clicked()
{
    ui->tableWidget->clear();
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle("请选择需要导入的CSV文件");
    fileDialog->setDirectory(".");
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setViewMode(QFileDialog::Detail);//详细
    fileDialog->setDefaultSuffix("*.csv");
    //判断是否选择了文件
    if (fileDialog->exec() == QDialog::Accepted)
    {
        QString path = fileDialog->selectedFiles()[0];
        QMessageBox::information(NULL, "提示", "你选择了 "+path);

        //此处创建导入用户的线程
        progress=0;//首先初始化进度条为0
        //创建一个线程用于更新进度条
        Update_Progress_Thread *UPT=new Update_Progress_Thread(this);
        connect(UPT,SIGNAL(resultReady(int)),this,SLOT(progressValue(int)));
        connect(UPT,SIGNAL(finished()),UPT,SLOT(deleteLater()));//线程结束后自动销毁
        UPT->start();

        //创建一个线程用于从硬盘读入csv文件
        Read_User_CSV *RUC=new Read_User_CSV(path,ui->comboBox->currentIndex()+1,this);
        connect(RUC,SIGNAL(resultReady(int,QString,int)),this,SLOT(slot_after_readCSV(int,QString,int)));
        connect(RUC,SIGNAL(finished()),RUC,SLOT(deleteLater()));//线程结束后自动销毁
        RUC->start();
    }
    else
    {
        QMessageBox::information(NULL, "错误", "你未选择任何文件，请重新选择！");
        ui->lineEdit->setText("请点击右边的按钮选择文件导入...");
    }
}
