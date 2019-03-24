#include "student_schedule.h"
#include "ui_student_schedule.h"

Student_Schedule::Student_Schedule(basic currentStudent,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Student_Schedule)
{
    this->currentStudent=currentStudent;
    isRefresh=false;
    ui->setupUi(this);

}

Student_Schedule::~Student_Schedule()
{
    delete ui;
}

//刷新加载个人课表的按钮
void Student_Schedule::on_pushButton_clicked()
{
    int errorcode=querySchedule(currentStudent.ID.left(4),currentStudent.label);
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

        isRefresh=true;//成功刷新表格

    }
    else if(errorcode==1)
        QMessageBox::information(NULL, "错误", "查询课程表失败 "+errorMessage);
}

//导出课表为CSV文件
void Student_Schedule::on_pushButton_2_clicked()
{
    if(isRefresh)
    {
        QString path = QFileDialog::getSaveFileName(this, "请选择存放CSV的路径", "./" +currentStudent.ID.left(4)+"_"+currentStudent.label+"_"+currentStudent.name+"_"+"课程表", tr("CSV file (*.csv)"));
        if (!path.isEmpty())
        {

            QTextCodec *code = QTextCodec::codecForName("GB2312");
            string tmp = code->fromUnicode(path).data();
            ofstream outfile(tmp.c_str(),ios::out);
            if(!outfile)
            {
                QMessageBox::information(NULL, "失败！", "在以下路径无法创建文件 "+path);
            }
            else
            {
                QTextCodec *code = QTextCodec::codecForName("GB2312");
                string tmp = code->fromUnicode("星期几,上课时间段,课程名,课程号,任教老师,上课地点,上课周").data();
                outfile<<tmp<<endl;
                for(vector<schedule>::iterator it=scheduleInfo.begin();it!=scheduleInfo.end();it++)
                {
                    outfile<<code->fromUnicode(it->TDAY).data()<<","<<code->fromUnicode(it->TTIME).data()<<","<<code->fromUnicode(it->CNAME).data()<<","<<code->fromUnicode(it->CNO).data()<<","<<code->fromUnicode(it->TNAME).data()<<","<<code->fromUnicode(it->TPLACE).data()<<","<<code->fromUnicode(it->SWEEK).data()<<endl;
                }
                outfile.close();
                QMessageBox::information(NULL, "成功！", "结果已经保存到以下文件 "+path);
            }
        }
        else
        {
            QMessageBox::information(NULL, "错误！", "你未选择保存到哪个路径！");
        }
        isRefresh=false;
    }
    else
    {
        QMessageBox::information(NULL, "错误", "在导出课表的时候需要先刷新一下课表，请点击“刷新个人课表”按钮。");
    }
}
