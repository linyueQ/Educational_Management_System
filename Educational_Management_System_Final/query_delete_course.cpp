#include "query_delete_course.h"
#include "ui_query_delete_course.h"

query_delete_course::query_delete_course(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::query_delete_course)
{
    ui->setupUi(this);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//把列表设置为只读

    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);//设置widget菜单项的显示方法
    menu = new QMenu(ui->tableWidget);
    del = new QAction("删除",this);

    //绑定右键弹出菜单事件
    connect(ui->tableWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(menu_requested(QPoint)));

    //绑定右键菜单点击后删除课程事件
    connect(del, SIGNAL(triggered()), this, SLOT(del_course()));

    //绑定按下回车点击确定查询按钮
    connect(ui->lineEdit, SIGNAL(returnPressed()), ui->pushButton, SIGNAL(clicked()), Qt::UniqueConnection);
    ui->tableWidget->clear();

}

void query_delete_course::menu_requested(QPoint pos)
{
    //先获取要被删除的课程的课程号
    QTableWidgetItem* sel=ui->tableWidget->itemAt(pos);
    sel_CNO=ui->tableWidget->item(sel->row(),0)->text();

    menu->addAction(del);
    menu->exec(QCursor::pos());
}

//该函数用于从课程列表里面删除课程，然后刷新显示列表
void query_delete_course::del_course()
{
    //QMessageBox::information(NULL, "OK", "点击删除成功，点击的课程号为"+sel_CNO);
    int reply;
    reply = QMessageBox::warning(this, "！重要提示！", "你确定要从系统中删除该条信息吗?",
                                 QMessageBox::Yes , QMessageBox::No);
    if(reply==QMessageBox::Yes)
    {
        if(delInfoFromDB(sel_CNO))
        {
            QMessageBox::information(NULL, "OK", "点击删除成功，被删除的课程号为"+sel_CNO);
            on_pushButton_clicked();
        }
    }
}

query_delete_course::~query_delete_course()
{
    delete ui;
}

void query_delete_course::on_pushButton_clicked()
{
    ui->tableWidget->clear();
    QString keywords="";
    //获取用户输入的关键字
    keywords=ui->lineEdit->text();

    int errorcode=queryCourse(keywords);
    if(errorcode==0)
    {
        ui->tableWidget->clear();//每次使用表格前先清除上次留下来的内容
        ui->tableWidget->setColumnCount(4); //设置列数

        //如果查询成功，则展示结果到表格上面
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
            ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString(it->CNO.trimmed())));
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString(it->CNAME.trimmed())));
            ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString::number(it->CREDIT,'g',6)));
            //判断课程性质
            if(it->FLAG==1)
            {
                ui->tableWidget->setItem(i,3,new QTableWidgetItem("公共必修课"));
            }
            if(it->FLAG==2)
            {
                ui->tableWidget->setItem(i,3,new QTableWidgetItem("专业必修课"));
            }
            if(it->FLAG==3)
            {
                ui->tableWidget->setItem(i,3,new QTableWidgetItem("专业选修课"));
            }
            if(it->FLAG==4)
            {
                ui->tableWidget->setItem(i,3,new QTableWidgetItem("通选课"));
            }

            //ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::number(it->FLAG,'g',6)));
            i++;
        }
        ui->label_sum->setText("一共查询出 "+QString::number(i,10)+" 条记录");
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
    else if(errorcode==1)
        QMessageBox::information(NULL, "错误", "查询的时候发生错误，错误信息："+errorMessage);
}

//删除所有的课程信息
void query_delete_course::on_pushButton_2_clicked()
{
    int reply;
    reply = QMessageBox::warning(this, "！重要提示！", "您正在执行高危操作，你确定要删库跑路吗?",
                                 QMessageBox::Yes , QMessageBox::No);
    if(reply==QMessageBox::Yes)
    {
        int reply2;
        reply2 = QMessageBox::question(this, "！重要提示！", "最后确认，你真的要从数据库删除所有的课程信息吗?",
                                     QMessageBox::Yes , QMessageBox::No);
        if(reply2==QMessageBox::Yes)
        {
            //开始删除课程列表
            QSqlQuery query;
            if(!query.exec("DELETE FROM COURSE"))
            {
                QMessageBox::information(this,"提示",
                "清空课程列表失败，你看着办吧："+query.lastError().text());
            }
            QMessageBox::information(this,"提示",
            "你已经成功删库，现在准备跑路吧，祝您生活愉快~~");
        }
        else return;
    }
    else return;
}
//导出查询结果为CSV文件
void query_delete_course::on_pushButton_3_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, "请选择存放CSV的路径", "./" + ui->lineEdit->text()+"列表查询结果", tr("CSV file (*.csv)"));
    if (!path.isEmpty())
    {
        if(outCSVfromTable(path,ui->tableWidget,"课程号,课程名,学分数,课程性质"))
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
        QMessageBox::information(NULL, QString::fromLocal8Bit("错误！"), QString::fromLocal8Bit("你未选择保存到哪个路径！"));
    }
}
