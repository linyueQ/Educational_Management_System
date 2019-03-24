#include "add_schedule.h"
#include "ui_add_schedule.h"

add_schedule::add_schedule(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_schedule)
{
    ui->setupUi(this);
    ui->lineEdit->setText("请选择文件夹╭(╯_╰)╮");
    ui->tableWidget->clear();
}

add_schedule::~add_schedule()
{
    delete ui;
}

//该函数在确定导入按钮点击时调用，用来创建SC表（因为学生表和课程表以及课程列表存在就可以生成SC表了）
int add_schedule::make_SC()
{
    //创建SC表，需要遍历整个scheduleInfo的记录
    //步骤：
    //1.首先连接STUDENT和TIMETABLE表；
    //2.然后直接把每条记录的学号、课程号提取出来放到SCInfo中，然后随机生成成绩并且排序（强行生成成绩……）
    //3.最后再把SCInfo里面的所有数据存入数据库中
    QSqlQuery query;

    QString q="insert into SC(SNO,CNO) SELECT distinct STUDENT.SNO,TIMETABLE.CNO FROM STUDENT,TIMETABLE WHERE SUBSTR(STUDENT.SNO,1,4)=TIMETABLE.GRADE AND STUDENT.MNAME=TIMETABLE.MNAME";
    if(!query.exec(q))
    {
        errorMessage=query.lastError().text();
        QMessageBox::information(NULL, "error", "STUDENT和TIMETABLE连接失败，信息插入失败"+errorMessage);
        return 1;
    }
    query.clear();
    if(!query.exec("update SC set SCORE=dbms_random.value(0,100)"))
    {
        QMessageBox::information(NULL, "error", "随机生成学生成绩出错"+query.lastError().text());
        return 1;
    }

    /*QString q="SELECT DISTINCT STUDENT.SNO,CNO FROM STUDENT,TIMETABLE WHERE SUBSTR(STUDENT.SNO,1,4)=TIMETABLE.GRADE AND STUDENT.MNAME=TIMETABLE.MNAME";
    if(!query.exec(q))
    {
        QMessageBox::information(NULL, "error", "STUDENT和TIMETABLE连接失败");
        return 1;
    }
    int i=0;
    while(query.next())
    {
        tmp.SNO=query.value(0).toString();
        tmp.CNO=query.value(1).toString();
        if(i%2==0) i+=rand();
        tmp.SCORE=i%100;//随机生成成绩

        SCInfo.push_back(tmp);
    }*/


    //SCInfo处理完，现在就要把里面的SNO和CNO数据入库
    if(writeInfoToDB_4())
    {
        QMessageBox::information(NULL, "error", "写入SC表时发生错误");
        return 1;
    }
    return 0;
}

void add_schedule::on_pushButton_clicked()
{
    ui->tableWidget->clear();
    //每次打开文件时都清空scheduleInfo，后面就不能再清空了，因为有多个文件，必须全部课程表文件读完写入数据库
    scheduleInfo.clear();
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle("请选择课程表所在文件夹");
    fileDialog->setDirectory(".");
    fileDialog->setFileMode(QFileDialog::Directory);//设置选择的对象为文件夹路径
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setViewMode(QFileDialog::Detail);//详细

    if (fileDialog->exec() == QDialog::Accepted)
    {
        QString path = fileDialog->selectedFiles()[0];
        QMessageBox::information(NULL, "提示", "你选择了 "+path);

        path = path.replace(QRegExp("\\/"), "\\");
        qDebug()<<path<<endl;

        QTextCodec *code = QTextCodec::codecForName("GB2312");
        string folderpath = code->fromUnicode(path).data();
        string filetype="*.csv";
        string filepath[1000];
        int num=0;//记录文件夹里面文件的数目

        showdir(folderpath,filetype,filepath,num);//获取了folderpath里面所有.csv文件的路径

        /*//测试一下
        ofstream a("E:\\Desktop\\1.txt",ios::out);
        for(int i=0;i<num;i++)
        {
            a<<filepath[i]<<'\n';
        }
        a.close();*/

        char fp[300];//InDatFromCSV函数只能用char数组作为参数
        strcpy(fp,filepath[0].c_str());//因为只显示第一张课表所以这里我只打开第一个文件的路径获取数据

        //这里我只输出第一个课表，供用户观看
        if(InDatFromCSV3(fp))
        {
            //下面开始读取文件，显示到tableWidget里面（只显示第一个读到的文件）
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
            }

            //下面是表的美化（格式控制）
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
        else
            QMessageBox::information(NULL, "错误", "从CSV导入如下文件的时候发生错误！无法打开文件"+path);

        //显示完第一个课程表以后，就把所有课程表记录到scheduleInfo里面
        for(int i=1;i<num;i++)//从1开始，因为0号文件已经读取过了
        {
            strcpy(fp,filepath[i].c_str());
            if(InDatFromCSV3(fp));
            else
                QMessageBox::information(NULL, "错误", "从CSV导入如下文件的时候发生错误！无法打开文件"+path);
        }
    }
    else
    {
        QMessageBox::information(NULL, "错误", "你未选择任何文件，请重新选择！");
    }
}

//这是确认导入按钮
void add_schedule::on_pushButton_2_clicked()
{
    int errorcode=writeInfoToDB_2();
    if(errorcode==0)
        QMessageBox::information(this,"OK","Insert the data successfully!");
    else if(errorcode==1)
        QMessageBox::information(this,"Fail",
                                 "Failed to write information into DB!，the error message:"+errorMessage);

    //下面开始创建SC表
    QMessageBox::information(this,"提示","课程表导入成功!!现在开始生成SC表!!");
    if(make_SC())
    {
        QMessageBox::information(this,"Fail",
                                 "创建SC表失败，原因："+errorMessage);
    }
    else
    {
        QMessageBox::information(this,"提示","SC表创建成功！！");
    }


}
