#include "studentlist.h"
#include "ui_studentlist.h"



studentlist::studentlist(basic currtea,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::studentlist)
{
    ui->setupUi(this);
    this->currTea=currtea;
    connect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(showstudentlist(QTableWidgetItem *	)));//获取点击的item，触发事件showstudentlist

}



//点击展示班级的按钮
void studentlist::on_pushButton_clicked()
{

    ui->tableWidget->clear();
    ui->tableWidget_2->clear();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//给表格设置不可编辑
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);//给表格设置不可编辑
    //获取老师id
    QString tno=currTea.ID.trimmed();//假设老师id
    int errorcode=classnamelist(tno);
    if(errorcode==0)
    {
        ui->tableWidget->clear();
        ui->tableWidget->setColumnCount(4);
        QStringList header;//设置表头内容
        header<<"年级"<<"班级"<<"课程号"<<"课程名";


        vector<teachinf3>::iterator begin1;//定义遍历器的开始
        vector<teachinf3>::iterator end1;//定义遍历器的结束


        //该老师所教课程的开始结束遍历器设置
        begin1=teachcourse3.begin();
        end1=teachcourse3.end();

        ui->tableWidget->setRowCount(teachcourse3.size());
        //qDebug()<<teachcourse3.size();
        ui->tableWidget->setHorizontalHeaderLabels(header);

        int i=0;
        //设置表格内容
        vector<teachinf3>::iterator it1=begin1;
        for(;it1!=end1;)
        {
            ui->tableWidget->setItem(i,0,new QTableWidgetItem(it1->GRADE));
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(it1->MNAME));
            ui->tableWidget->setItem(i,2,new QTableWidgetItem(it1->CNO));
            ui->tableWidget->setItem(i,3,new QTableWidgetItem(it1->CNAME));
            i++;
            it1++;

        }
        ui->tableWidget->repaint();//让表格刷新显示

    }
    else if(errorcode==1)
        QMessageBox::information(NULL, "错误", "查询的时候发生错误，错误信息："+errorMessage);

}

//点击班级展示学生名单
void studentlist::showstudentlist( QTableWidgetItem* item)
{

    select_row1=-1;
    QString grade="";
    QString mname="";
    QString cno="";
    if(item!=NULL)
    {
        select_row1=item->row();

        //selected_NO1=ui->tableWidget->item(select_row1,0)->text();//获取当前选中的班级名称
        // grade=grade+selected_NO1.mid(0,4);
        //mname=mname+selected_NO1.mid(5);

        grade=ui->tableWidget->item(select_row1,0)->text().trimmed();
        mname=ui->tableWidget->item(select_row1,1)->text().trimmed();
        cno=ui->tableWidget->item(select_row1,2)->text().trimmed();
    }

    ui->tableWidget_2->clear();
    //进行查询

    int errorcode=searchstudentlist(grade,mname,cno);
    if(errorcode==0)
    {
        ui->tableWidget_2->clear();
        ui->tableWidget_2->setColumnCount(2); //设置列数
        //如果查询成功，则展示结果到表格上面
        QStringList header;//设置表头内容
        vector<teachinf4>::iterator begin;//定义遍历器的开始
        vector<teachinf4>::iterator end;//定义遍历器的结束

        header<<"学号"<<"姓名";
        begin=teachcourse4.begin();
        end=teachcourse4.end();
        ui->tableWidget_2->setRowCount(teachcourse4.size());
        ui->tableWidget_2->setHorizontalHeaderLabels(header);
        int i=0;
        //设置表格内容
        for(vector<teachinf4>::iterator it=begin;it!=end;it++)
        {

            ui->tableWidget_2->setItem(i,0,new QTableWidgetItem(QString(it->SNO)));
            ui->tableWidget_2->setItem(i,1,new QTableWidgetItem(QString(it->SNAME)));
            i++;
        }

        ui->tableWidget_2->repaint();//让表格刷新显示

    }
    else if(errorcode==1)
        QMessageBox::information(NULL, "错误", "查询的时候发生错误，错误信息："+errorMessage);
}

studentlist::~studentlist()
{
    delete ui;
}

//导出学生名单为CSV文件
void studentlist::on_pushButton_2_clicked()
{
    QString grade=ui->tableWidget->item(select_row1,0)->text().trimmed();
    QString mname=ui->tableWidget->item(select_row1,1)->text().trimmed();
    QString cno=ui->tableWidget->item(select_row1,2)->text().trimmed();

    QString path = QFileDialog::getSaveFileName(this, "请选择存放CSV的路径", "./"+grade+"_"+mname+"_"+cno+"_"+"点名册", tr("CSV file (*.csv)"));
    if (!path.isEmpty())
    {
        if(outCSVfromTable(path,ui->tableWidget_2,"学号,姓名"))
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
