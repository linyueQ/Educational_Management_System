#include "student_query_score.h"
#include "ui_student_query_score.h"

Student_Query_Score::Student_Query_Score(basic currentStudent,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Student_Query_Score)
{
    this->currentStudent=currentStudent;
    isRefresh=false;
    ui->setupUi(this);
    ui->label_GPA->setText("暂无GPA绩点信息");
}

Student_Query_Score::~Student_Query_Score()
{
    delete ui;
}

//刷新成绩表
void Student_Query_Score::on_pushButton_clicked()
{
    QSqlQuery query;
    //"课程名,课程号,学分数,成绩,单科班级排名"
    if(query.exec("select COURSE.CNAME,SC.CNO,COURSE.CREDIT,SC.SCORE,SC.SRANK from SC,COURSE where SC.CNO=COURSE.CNO and SC.SNO='"+currentStudent.ID+"'"))
    {
        SCInfo.clear();
        SC sctmp;
        while(query.next())
        {
            sctmp.CNAME=query.value(0).toString().trimmed();
            sctmp.CNO=query.value(1).toString().trimmed();
            sctmp.CREDIT=query.value(2).toFloat();
            sctmp.SCORE=query.value(3).toFloat();
            sctmp.SRANK=query.value(4).toInt();
            SCInfo.push_back(sctmp);
        }
        QStringList header;
        header<<"课程名"<<"课程号"<<"学分数"<<"成绩"<<"单科班级排名";
        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(SCInfo.size());
        ui->tableWidget->setColumnCount(5);
        ui->tableWidget->setHorizontalHeaderLabels(header);
        int i=0;
        for(vector<SC>::iterator it=SCInfo.begin();it!=SCInfo.end();it++)
        {
            ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString(it->CNAME)));
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString(it->CNO)));
            ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString::number(it->CREDIT,'f',2)));
            ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::number(it->SCORE,'f',2)));
            ui->tableWidget->setItem(i,4,new QTableWidgetItem(QString::number(it->SRANK,10)));
            i++;
        }
        ui->tableWidget->repaint();
        isRefresh=true;

        //查询一下这名学生的GPA有没有出来
        query.clear();
        if(query.exec("select SGPA from GPA where SNO='"+currentStudent.ID+"'"))
        {
            if(query.next())
            {
                ui->label_GPA->setText("GPA绩点为："+QString::number(query.value(0).toFloat(),'f',3));
            }
            else
            {
                ui->label_GPA->setText("暂无GPA绩点信息");
            }
        }
        else
        {
            ui->label_GPA->setText("暂无GPA绩点信息");
        }
    }
    else
    {
        errorMessage=query.lastError().text();
        QMessageBox::information(NULL, "错误", "在查询数据库成绩表的时候发生错误，错误信息："+errorMessage);
    }
}

//导出成绩表为CSV文件
void Student_Query_Score::on_pushButton_2_clicked()
{
    if(isRefresh)
    {
        QString path = QFileDialog::getSaveFileName(this, "请选择存放CSV的路径", "./" +currentStudent.ID.left(4)+"_"+currentStudent.label+"_"+currentStudent.name+"_"+"成绩表", tr("CSV file (*.csv)"));
        if (!path.isEmpty())
        {
            if(outCSVfromTable(path,ui->tableWidget,"课程名,课程号,学分数,成绩,单科班级排名"))
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
        isRefresh=false;
    }
    else
    {
        QMessageBox::information(NULL, "错误", "在导出成绩表的时候需要先刷新一下成绩表，请点击“刷新个人成绩表”按钮。");
    }
}
