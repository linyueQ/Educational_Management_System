#include "student_score.h"
#include "ui_student_score.h"
#include "base.h"


student_score::student_score(basic currtea,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::student_score)
{
    ui->setupUi(this);
    this->currTea=currtea;
    //连接 点击班级表格 触发的函数
    connect(ui->tableWidget_class,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(showstudentscore(QTableWidgetItem *)));//获取点击的item，触发事件showstudentlist
    //连接 修改成绩表 触发的函数
    connect(ui->tableWidget_score,SIGNAL(itemChanged(QTableWidgetItem *)),this,SLOT(tableChangeSignal(QTableWidgetItem *)));//获取被修改的的item，触发事件tableChangeSignal
    isEdit=false;
}

student_score::~student_score()
{
    delete ui;

}

//点击展示班级的按钮
void student_score::on_pushButton_expandclass_clicked()
{
    isEdit=false;
    ui->tableWidget_class->clear();
    ui->tableWidget_score->clear();
    ui->tableWidget_class->setEditTriggers(QAbstractItemView::NoEditTriggers);//给表格设置不可编辑

    //获取老师id
    QString tno=currTea.ID.trimmed();//假设老师id
    int errorcode=classnamelist(tno);
    if(errorcode==0)
    {
        ui->tableWidget_class->clear();
        ui->tableWidget_class->setColumnCount(4);
        QStringList header;//设置表头内容
        header<<"年级"<<"班级"<<"课程号"<<"课程名";


        vector<teachinf3>::iterator begin1;//定义遍历器的开始
        vector<teachinf3>::iterator end1;//定义遍历器的结束


        //该老师所教课程的开始结束遍历器设置
        begin1=teachcourse3.begin();
        end1=teachcourse3.end();

        ui->tableWidget_class->setRowCount(teachcourse3.size());
        //qDebug()<<teachcourse3.size();
        ui->tableWidget_class->setHorizontalHeaderLabels(header);

        int i=0;
        //设置表格内容
        vector<teachinf3>::iterator it1=begin1;
        for(;it1!=end1;)
        {
            ui->tableWidget_class->setItem(i,0,new QTableWidgetItem(it1->GRADE));
            ui->tableWidget_class->setItem(i,1,new QTableWidgetItem(it1->MNAME));
            ui->tableWidget_class->setItem(i,2,new QTableWidgetItem(it1->CNO));
            ui->tableWidget_class->setItem(i,3,new QTableWidgetItem(it1->CNAME));
            i++;
            it1++;

        }
        ui->tableWidget_class->repaint();//让表格刷新显示

    }
    else if(errorcode==1)
        QMessageBox::information(NULL, "错误", "查询的时候发生错误，错误信息："+errorMessage);
}

//双击了某个班级之后，下面展示对应的SC表
void student_score::showstudentscore(QTableWidgetItem *item)
{
    isEdit=false;
    select_row1=-1;
    QString grade="";
    QString mname="";
    QString cno="";
    QString cname="";
    if(item!=NULL)
    {
        select_row1=item->row();

        //selected_NO1=ui->tableWidget->item(select_row1,0)->text();//获取当前选中的班级名称
        // grade=grade+selected_NO1.mid(0,4);
        //mname=mname+selected_NO1.mid(5);

        grade=ui->tableWidget_class->item(select_row1,0)->text().trimmed();
        mname=ui->tableWidget_class->item(select_row1,1)->text().trimmed();
        cno=ui->tableWidget_class->item(select_row1,2)->text().trimmed();
        cname=ui->tableWidget_class->item(select_row1,3)->text().trimmed();
    }

    ui->tableWidget_score->clear();
    //进行查询
    teachcourse4.clear();
    int errorcode=searchstudentlist(grade,mname,cno);
    if(errorcode==0)
    {
        ui->tableWidget_score->clear();
        ui->tableWidget_score->setColumnCount(5); //设置列数
        //如果查询成功，则展示结果到表格上面
        QStringList header;//设置表头内容
        vector<teachinf4>::iterator begin;//定义遍历器的开始
        vector<teachinf4>::iterator end;//定义遍历器的结束

        header<<"学号"<<"姓名"<<"课程名"<<"成绩"<<"班级单科排名";
        begin=teachcourse4.begin();
        end=teachcourse4.end();
        ui->tableWidget_score->setRowCount(teachcourse4.size());
        ui->tableWidget_score->setHorizontalHeaderLabels(header);
        int i=0;
        //设置表格内容
        for(vector<teachinf4>::iterator it=begin;it!=end;it++)
        {

            ui->tableWidget_score->setItem(i,0,new QTableWidgetItem(QString(it->SNO)));
            ui->tableWidget_score->item(i,0)->setFlags(Qt::ItemIsEnabled);//除了成绩可以让老师手动输入，其它表项都不可以编辑
            ui->tableWidget_score->setItem(i,1,new QTableWidgetItem(QString(it->SNAME)));
            ui->tableWidget_score->item(i,1)->setFlags(Qt::ItemIsEnabled);
            ui->tableWidget_score->setItem(i,2,new QTableWidgetItem(QString(cname)));
            ui->tableWidget_score->item(i,2)->setFlags(Qt::ItemIsEnabled);
            ui->tableWidget_score->setItem(i,3,new QTableWidgetItem(QString::number(it->score, 'f', 2)));
            if(it->srank<10)
                ui->tableWidget_score->setItem(i,4,new QTableWidgetItem("0"+QString::number(it->srank, 10)));
            else
                ui->tableWidget_score->setItem(i,4,new QTableWidgetItem(QString::number(it->srank, 10)));
            ui->tableWidget_score->item(i,4)->setFlags(Qt::ItemIsEnabled);
            i++;
        }


        //此处定义点击表头排序的功能
        QHeaderView *headerGoods = ui->tableWidget_score->horizontalHeader();
        //SortIndicator为水平标题栏文字旁边的三角指示器
        headerGoods->setSortIndicator(0, Qt::AscendingOrder);
        headerGoods->setSortIndicatorShown(true);
        headerGoods->setSectionsClickable(true);
        connect(headerGoods, SIGNAL(sectionClicked(int)), ui->tableWidget_score, SLOT(sortByColumn(int)));

        ui->tableWidget_score->repaint();//让表格刷新显示
        isEdit=true;

        //下面开始计算平均分和及格率
        float scoresum=0.0;//整个班的总分，用于计算平均分
        int passSum=0.0;//及格人数
        for(vector<teachinf4>::iterator it=teachcourse4.begin();it!=teachcourse4.end();it++)
        {
            scoresum=scoresum+it->score;
            if(it->score>=60.0)
                passSum++;
        }
        //计算平均分和及格率，并展示到对应位置上
        double average=scoresum/(teachcourse4.size()*1.0);
        double passRate=(passSum*1.0)/(teachcourse4.size()*1.0);
        ui->label_average->setText("班级平均分"+QString::number(average,'f',2));
        ui->label_passrate->setText("班级及格率"+QString::number(passRate,'f',2));
    }
    else if(errorcode==1)
        QMessageBox::information(NULL, "错误", "查询的时候发生错误，错误信息："+errorMessage);
}

//表格内容改变要执行的函数，更新vector里面的成绩
void student_score::tableChangeSignal(QTableWidgetItem *item)
{
    if(isEdit&&item!=NULL)
    {
        if(item->column()==3)
        {
            //遍历寻找这个学生，修改其在容器里面的成绩
            for(vector<teachinf4>::iterator it=teachcourse4.begin();it!=teachcourse4.end();it++)
            {
                if(ui->tableWidget_score->item(item->row(),0)->text().trimmed().compare(it->SNO)==0)
                {
                    float old_score=item->text().toFloat();
                    if(old_score>=0&&old_score<=100)
                    {
                        it->score=old_score;
                        qDebug()<<it->SNO<<it->score<<endl;
                    }
                    else
                    {
                        QMessageBox::information(NULL, "错误", "您输入的 "+it->SNAME+" 的成绩不合法，请确保成绩的范围在0~100分");
                        ui->tableWidget_score->item(item->row(),3)->setText(QString::number(it->score,'f',2));
                    }
                    break;//找到这个学生的成绩之后就可以退出循环
                }
            }
        }
    }
}

//用于vector<teachinf4> 按成绩降序排序
bool compare_score(teachinf4 t1,teachinf4 t2)
{
    return t1.score>t2.score;
}

//点击确定导入成绩的按钮
void student_score::on_pushButton_okinscore_clicked()
{
    isEdit=false;//当点击了确认导入之后，锁定vector的修改开关，不能再让用户修改vector里面的成绩了
    sort(teachcourse4.begin(),teachcourse4.end(),compare_score);//对向量里面的元素按成绩进行降序排序，然后回填每位学生的排名
    int i=1;
    QSqlQuery query;
    float scoresum=0.0;//整个班的总分，用于计算平均分
    int passSum=0.0;//及格人数
    for(vector<teachinf4>::iterator it=teachcourse4.begin();it!=teachcourse4.end();it++)
    {
        it->srank=i++;
        scoresum=scoresum+it->score;
        if(it->score>=60.0)
            passSum++;
        //一边更新排名一边写入到数据库
        query.clear();
        if(!query.exec("update SC set SCORE = "+QString::number(it->score,'f',4)+", SRANK = "+QString::number(it->srank,10)+" where SNO = '"+it->SNO+"' and CNO = '"+it->CNO+"'"))
        {
            errorMessage=query.lastError().text();
            QMessageBox::information(NULL, "错误", "更新学生成绩的时候发生错误，错误信息："+errorMessage);
        }
    }
    //计算平均分和及格率，并展示到对应位置上
    double average=scoresum/(teachcourse4.size()*1.0);
    double passRate=(passSum*1.0)/(teachcourse4.size()*1.0);
    ui->label_average->setText("班级平均分"+QString::number(average,'f',2));
    ui->label_passrate->setText("班级及格率"+QString::number(passRate,'f',2));

    QMessageBox::information(NULL, "完成", "完成更新学生的成绩！");

}

//导出成绩表为CSV文件
void student_score::on_pushButton_clicked()
{
    QString grade=ui->tableWidget_class->item(select_row1,0)->text().trimmed();
    QString mname=ui->tableWidget_class->item(select_row1,1)->text().trimmed();
    QString cno=ui->tableWidget_class->item(select_row1,2)->text().trimmed();

    QString path = QFileDialog::getSaveFileName(this, "请选择存放CSV的路径", "./"+grade+"_"+mname+"_"+cno+"_"+"成绩表", tr("CSV file (*.csv)"));
    if (!path.isEmpty())
    {
        if(outCSVfromTable(path,ui->tableWidget_score,"学号,姓名,课程名,成绩,班级单科排名"))
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
