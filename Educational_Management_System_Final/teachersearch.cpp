
#include "teachersearch.h"
#include "ui_teachersearch.h"

teachersearch::teachersearch(basic currtea,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::teachersearch)
{
    ui->setupUi(this);
    this->currTea=currtea;
    //connect( ui->pushButton, SIGNAL(clicked()), this,SLOT(on_pushButton_clicked()));
    //connect(ui->lineEdit, SIGNAL(returnPressed()), ui->pushButton, SIGNAL(clicked()), Qt::UniqueConnection);

}




teachersearch::~teachersearch()
{
    delete ui;
}


//点击确认查询按钮
void teachersearch::on_pushButton_clicked()
{
    ui->tableWidget->clear();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//给表格设置不可编辑


    //qDebug()<<"1";
    //获取老师id
    QString tno=currTea.ID.trimmed();
    int errorcode=queryallcourse(tno);

    if(errorcode==0)
    {
        ui->tableWidget->clear();//每次使用表格前先清除上次留下来的内容
        ui->tableWidget->setColumnCount(9);
        QStringList header;//设置表头内容
        header<<"课程号,课程名称,课程性质,上课周数,上课时间,上课地点,学分,专业名,年级";


        vector<teachinf1>::iterator begin1;//定义遍历器的开始
        vector<teachinf1>::iterator end1;//定义遍历器的结束

        vector<teachinf2>::iterator begin2;//定义遍历器的开始
        vector<teachinf2>::iterator end2;//定义遍历器的结束

        //该老师所教课程的开始结束遍历器设置
        begin1=teachcourse1.begin();
        end1=teachcourse1.end();

        begin2=teachcourse2.begin();
        end2=teachcourse2.end();

        qDebug()<<teachcourse1.size();
        qDebug()<<teachcourse2.size();
        if(teachcourse1.size()!=teachcourse2.size())
        {
            //报错

        }
        else
        {
            //qDebug()<<"4";
            ui->tableWidget->setRowCount(teachcourse1.size());
            ui->tableWidget->setHorizontalHeaderLabels(header);
            int i=0;
            //设置表格内容
            vector<teachinf1>::iterator it1=begin1;
            vector<teachinf2>::iterator it2=begin2;
            for(;it1!=end1&&it2!=end2;)
            {

                ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString(it1->CNO)));
                ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString(it2->CNAME)));
                //判断课程性质
                QString str1="";
                QString str2="";
                QString str3="";
                QString str4="";
                str1=str1+"1";
                str2=str2+"2";
                str3=str3+"3";
                str4=str4+"4";


                if(QString(it2->FLAG)==str1)
                {
                    ui->tableWidget->setItem(i,2,new QTableWidgetItem("公共必修课"));
                }
                if(QString(it2->FLAG)==str2)
                {
                    ui->tableWidget->setItem(i,2,new QTableWidgetItem("专业必修课"));
                }
                if(QString(it2->FLAG)==str3)
                {
                    ui->tableWidget->setItem(i,2,new QTableWidgetItem("专业选修课"));
                }
                if(QString(it2->FLAG)==str4)
                {
                    ui->tableWidget->setItem(i,2,new QTableWidgetItem("通选课"));
                }
                ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString(it1->SWEEK)));
                qDebug()<<QString(it1->SWEEK);
                ui->tableWidget->setItem(i,4,new QTableWidgetItem(QString(it1->TIMEE)));
                qDebug()<<QString(it1->TIMEE);
                ui->tableWidget->setItem(i,5,new QTableWidgetItem(QString(it1->TPLACE)));
                ui->tableWidget->setItem(i,6,new QTableWidgetItem(QString(it2->CREDIT)));
                ui->tableWidget->setItem(i,7,new QTableWidgetItem(QString(it1->MNAME)));
                ui->tableWidget->setItem(i,8,new QTableWidgetItem(QString(it1->GRADE)));
                i++;
                it1++;
                it2++;
            }
            ui->tableWidget->repaint();//让表格刷新显示

        }
    }
    else if(errorcode==1)
        QMessageBox::information(NULL, "错误", "查询的时候发生错误，错误信息："+errorMessage);
}

//导出上课安排表为CSV文件
void teachersearch::on_pushButton_2_clicked()
{

    QString path = QFileDialog::getSaveFileName(this, "请选择存放CSV的路径", "./"+currTea.name+"_上课安排表", tr("CSV file (*.csv)"));
    if (!path.isEmpty())
    {
        if(outCSVfromTable(path,ui->tableWidget,"课程号,课程名称,课程性质,上课周数,上课时间,上课地点,学分,专业名,年级"))
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
