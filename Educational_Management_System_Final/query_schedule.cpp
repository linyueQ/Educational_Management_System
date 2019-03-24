#include "query_schedule.h"
#include "ui_query_schedule.h"

query_schedule::query_schedule(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::query_schedule)
{
    ui->setupUi(this);
    /*//新建ui的时候需要先初始化下拉框（数据库建好以后用这个）
    QSqlQuery query;
    QString find="SELECT * FROM GLIST";
    if(!query.exec(find))
    {
        errorMessage=query.lastError().text();
        return 1;
    }
    for(int i=0;i<4;i++)
    {
        ui->comboBox->addItem(query.value(0).toString());
        query.next();
    }*/

    ui->comboBox->addItem("2015");
    ui->comboBox->addItem("2016");
    ui->comboBox->addItem("2017");
    ui->comboBox->addItem("2018");

    //现在初始化专业
    flush_m();

    //绑定按下回车点击确定查询按钮
    connect(ui->lineEdit, SIGNAL(returnPressed()), ui->pushButton, SIGNAL(clicked()), Qt::UniqueConnection);
    ui->tableWidget->clear();

}

query_schedule::~query_schedule()
{
    delete ui;
}

//用来获取所有的专业名，并加入到combobox2里面
void query_schedule::flush_m()
{
    QSqlQuery query;
    if(!query.exec("SELECT MNAME FROM MI"))
    {
        QMessageBox::information(NULL, "提示", "获取专业列表失败"+query.lastError().text());
        return;
    }
    while(query.next())
    {
        ui->comboBox_2->addItem(query.value(0).toString().trimmed());
    }
    return;
}

//这个是用来查询课程表的按键
void query_schedule::on_pushButton_clicked()
{
    ui->tableWidget->clear();
    QString keywords="";

    if(ui->checkBox->isChecked()==true)
    {
        //获取用户输入的关键字
        keywords=ui->lineEdit->text();
    }
    else
    {
        keywords=ui->comboBox_2->currentText();
    }

    QString year;//用来获取下拉框的年份
    year=ui->comboBox->currentText();

    int errorcode=querySchedule(year,keywords);
    if(errorcode==0)
    {
        ui->tableWidget->clear();//每次使用表格前先清除上次留下来的内容
        ui->tableWidget->setColumnCount(7); //设置列数
        ui->tableWidget->setRowCount(6);

        //如果数据导入成功，则展示到表格上面
        QStringList header1;//设置上表头内容
        QStringList header2;//设置左表头内容
        header1<<"星期一"<<"星期二"<<"星期三"<<"星期四"<<"星期五"<<"星期六"<<"星期天";
        header2<<"第1、2节"<<"第3、4节"<<"第5、6节"<<"第7节"<<"第8节"<<"第9、10、11节";
        ui->tableWidget->setHorizontalHeaderLabels(header1);
        ui->tableWidget->setVerticalHeaderLabels(header2);
        if(!showSchedule(ui->tableWidget))//显示课程表，并判断是否读取成功
        {
            QMessageBox::information(NULL, "错误", "读取某一课程的星期几或具体时间段或查询语句存在错误！无法显示课程表");
            return;
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
    else if(errorcode==1)
        QMessageBox::information(NULL, "错误", "查询课程表失败 "+errorMessage);
}

//这个按钮用来清空清空数据库中所有的课程表
void query_schedule::on_pushButton_2_clicked()
{
    int reply;
    reply = QMessageBox::warning(this, "！重要提示！", "您正在清空课程表，你确定要删库跑路吗?",
                                 QMessageBox::Yes , QMessageBox::No);
    if(reply==QMessageBox::Yes)
    {
        int reply2;
        reply2 = QMessageBox::question(this, "！重要提示！", "最后确认，你真的要删除数据库吗?",
                                     QMessageBox::Yes , QMessageBox::No);
        if(reply2==QMessageBox::Yes)
        {
            //开始删除课程表
            QSqlQuery query;
            if(!query.exec("DELETE FROM TIMETABLE"))
            {
                QMessageBox::information(this,"提示",
                "清空课程表失败，你看着办吧："+query.lastError().text());
            }
            QMessageBox::information(this,"提示",
            "你已经成功删库，现在准备跑路吧，祝您生活愉快~~");
        }
        else return;
    }
    else return;
}

//导出结果为CSv文件
void query_schedule::on_pushButton_3_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, "请选择存放CSV的路径", "./" + ui->comboBox->currentText()+ui->comboBox_2->currentText()+"课表查询结果", tr("CSV file (*.csv)"));
    if (!path.isEmpty())
    {
        if(outCSVfromTable(path,ui->tableWidget,"周一,周二,周三,周四,周五,周六,周日"))
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
