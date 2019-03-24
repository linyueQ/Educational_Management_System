#include "add_course.h"
#include "ui_add_course.h"

add_course::add_course(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_course)
{
    ui->setupUi(this);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//把列表设置为只读
    ui->lineEdit->setText("请选择文件o^v^o");
    ui->tableWidget->clear();
}

add_course::~add_course()
{
    delete ui;
}

void add_course::on_pushButton_clicked()
{
    ui->tableWidget->clear();
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle("请选择需要导入的CSV文件");
    fileDialog->setDirectory(".");//设置默认路径
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setViewMode(QFileDialog::Detail);//详细
    fileDialog->setDefaultSuffix("*.csv");
    if (fileDialog->exec() == QDialog::Accepted)
    {
        QString path = fileDialog->selectedFiles()[0];
        //QMessageBox::information(this,"OK","Insert the data successfully! "+path);
        QMessageBox::information(NULL, "测试", "你选择了 "+path);
        QTextCodec *code = QTextCodec::codecForName("GB2312");
        string tmp = code->fromUnicode(path).data();
        char filepath[300];
        strcpy(filepath, tmp.c_str());
        if(InDatFromCSV2(filepath))
        {
            QMessageBox::information(this,"OK","Get data successful! "+path);
            ui->tableWidget->clear();//每次使用表格前先清除上次留下来的内容
            ui->tableWidget->setColumnCount(4); //设置列数，课程列表也就4列
            //如果数据导入成功，则展示到表格上面
            QStringList header;//设置表头内容
            vector<course>::iterator begin;//定义遍历器的开始
            vector<course>::iterator end;//定义遍历器的结束

            //设置标题栏
            header<<"课程号"<<"课程名"<<"学分数"<<"课程类型";
            ui->tableWidget->setHorizontalHeaderLabels(header);

            //开始循环设置课程列表
            begin=courseInfo.begin();
            end=courseInfo.end();
            ui->tableWidget->setRowCount(courseInfo.size());//设置行数
            int i=0;
            for(vector<course>::iterator it=begin;it!=end;it++)
            {
                ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString(it->CNO)));
                ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString(it->CNAME)));
                ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString::number(it->CREDIT,'g',6)));
                ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::number(it->FLAG,'g',6)));
                i++;
            }
            //下面是tableWidget的格式调整
            ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //自适应列宽
            ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch); //自适应行高

            ui->tableWidget->setFrameShape(QFrame::NoFrame); //设置无边框
            ui->tableWidget->setShowGrid(true); //设置不显示格子线
            ui->tableWidget->verticalHeader()->setVisible(true); //设置垂直头不可见
            ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);  //可多选（Ctrl、Shift、  Ctrl+A都可以）
            ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
            //ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑

            ui->tableWidget->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
            ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
            //设置水平、垂直滚动条
            ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            ui->tableWidget->repaint();

            //此处定义点击表头排序的功能
            QHeaderView *headerGoods = ui->tableWidget->horizontalHeader();
            //SortIndicator为水平标题栏文字旁边的三角指示器
            headerGoods->setSortIndicator(0, Qt::AscendingOrder);
            headerGoods->setSortIndicatorShown(true);
            headerGoods->setSectionsClickable(true);
            connect(headerGoods, SIGNAL(sectionClicked(int)), ui->tableWidget, SLOT(sortByColumn(int)));

        }
        else QMessageBox::information(NULL, "错误", "在导入如下文件的时候发生错误！无法打开文件"+path);
    }
    else QMessageBox::information(NULL, "错误", "你未选择任何文件，请重新选择！");
}

void add_course::on_pushButton_2_clicked()
{
    int errorcode=writeInfoToDB_1();
    if(errorcode==0)
        QMessageBox::information(this,"OK","Insert the data successfully!");
    else if(errorcode==1)
        QMessageBox::information(this,"Fail",
        "Failed to write information into DB!，the error message:"+errorMessage);
}
