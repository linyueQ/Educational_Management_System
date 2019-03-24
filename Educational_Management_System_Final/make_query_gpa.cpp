#include "make_query_gpa.h"
#include "ui_make_query_gpa.h"

make_query_gpa::make_query_gpa(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::make_query_gpa)
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

    //绑定按下回车点击确定查询按钮
    connect(ui->lineEdit, SIGNAL(returnPressed()), ui->pushButton, SIGNAL(clicked()), Qt::UniqueConnection);
    ui->tableWidget->clear();
}

make_query_gpa::~make_query_gpa()
{
    delete ui;
}

//查看某专业的GPA
void make_query_gpa::on_pushButton_clicked()
{
    ui->tableWidget->clear();
    QString keywords="";
    //获取用户输入的关键字
    keywords=ui->lineEdit->text();

    QString year;//用来获取下拉框的年份
    year=ui->comboBox->currentText();


    int errorcode=queryGPA(year,keywords);
    //显示该专业学生的gpa：学号、姓名、专业名、gpa、排名
    if(errorcode==0)
    {
        ui->tableWidget->clear();//每次使用表格前先清除上次留下来的内容
        ui->tableWidget->setColumnCount(5); //设置列数
        ui->tableWidget->setRowCount(sGPA.size());

        //如果数据导入成功，则展示到表格上面
        QStringList header;//设置上表头内容
        header<<"学号"<<"姓名"<<"专业"<<"GPA"<<"专业排名";
        ui->tableWidget->setHorizontalHeaderLabels(header);

        int i=0;
        for(vector<show_gpa>::iterator it=sGPA.begin();it!=sGPA.end();it++)
        {
            ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString(it->SNO)));
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString(it->SNAME)));
            ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString(it->MNAME)));
            ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::number(it->SGPA,'g',6)));
            if(it->SRANK<10)
            {
                ui->tableWidget->setItem(i,4,new QTableWidgetItem("0"+QString::number(it->SRANK,'g',6)));
            }
            else ui->tableWidget->setItem(i,4,new QTableWidgetItem(QString::number(it->SRANK,'g',6)));
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

}

//生成所有学生的GPA
void make_query_gpa::on_pushButton_1_clicked()
{
    QSqlQuery query;

    vector<GPA> sameMajor;//这是用来暂时存一个专业的所有人的GPA，每统计完一个专业都要先得到排名，然后才可以加入GPAInfo里面
    GPA tmp;
    vector<cal_gpa> personal;//记录个人的所有SC的信息（再加上专业MNAME的信息）;
    cal_gpa psl;

    //先把SC表和COURSE表以及STUDENT表连接起来（连接STUDENT表是因为要获得专业，计算GPA的排名）
    if(!query.exec("SELECT SC.SNO,SC.CNO,MNAME,SCORE,CREDIT FROM SC,COURSE,STUDENT WHERE SC.CNO=COURSE.CNO AND SC.SNO=STUDENT.SNO ORDER BY SNO ASC"))
    {
        QMessageBox::information
                (NULL, "提示", "生成GPA失败，查询SC/COURSE/STUDENT表时发生错误，错误类型为："
                 +query.lastError().text());
        return;
    }
    //然后计算GPA（注意，GPA的排名只计算本专业的）
    int i=0;
    while(query.next())
    {
        psl.SNO=query.value(0).toString().trimmed();
        psl.CNO=query.value(1).toString().trimmed();
        psl.MNAME=query.value(2).toString().trimmed();
        psl.score=query.value(3).toFloat();
        psl.credit=query.value(4).toFloat();

        vector<cal_gpa>::iterator it=personal.begin();


        //在加入到personal之前要先比对一下，这个psl是不是同一个人的
        if(personal.size()==0)//一开始personal为空，不能进行比较
        {
            personal.push_back(psl);
            //qDebug()<<psl.SNO<<psl.CNO<<psl.MNAME<<psl.score<<psl.credit<<endl;

        }
        else if((psl.SNO).compare(it->SNO)==0)
        {
            personal.push_back(psl);
            //qDebug()<<psl.SNO<<psl.CNO<<psl.MNAME<<psl.score<<psl.credit<<endl;
        }
        else//如果不是，那么就要处理personal里面的东西，来计算一个学生的SGPA了，计算完以后记得要清空personal，然后再存入本次的psl
        {
            float gpa=0;

            for(vector<cal_gpa>::iterator it=personal.begin();it!=personal.end();it++)
            {
                gpa+=(it->score/100.0)*it->credit;
            }
            tmp.SNO=personal.begin()->SNO;
            tmp.SGPA=gpa;

            //在加入到temp之前，先要看看这个tmp学生是不是和上一个学生同一个专业
            //如果是，那么直接把这个学生的gpa存入sameMajor里面
            //如果不是，那么说明上一个专业的所有学生的gpa都计算出来了，需要进行排名
            //要把sameMajor的学生gpa进行排序并且存放到真正的GPA列表GPAInfo里面,把sameMajor清空
            //然后再把现在这个学生加入到空的sameMajor里面
            sameMajor.push_back(tmp);//把同一个专业的学生放到一个vector里面计算排名
            //如果第二个专业的第一条记录已经被读入，那么上一个专业的最后一条记录也已经被存入，可以开始计算排名了
            if(psl.MNAME.trimmed().compare(personal.begin()->MNAME.trimmed())!=0)
            {

                //qDebug()<<psl.MNAME<<" "<<personal.begin()->MNAME<<endl;


                //先对sameMajor的所有学生的gpa进行排序
                sort(sameMajor.begin(),sameMajor.end());//以gpa作为排序标准进行排序
                //排好序以后，给排名赋值，这条记录就完整了，可以直接加入到GPAInfo里面
                int j=1;
                for(vector<GPA>::iterator it=sameMajor.begin();it!=sameMajor.end();it++,j++)
                {
                    it->SRANK=j;
                    GPA tt;
                    tt.SNO=it->SNO;
                    tt.SGPA=it->SGPA;
                    tt.SRANK=it->SRANK;
                    //qDebug()<<tt.SNO<<" "<<tt.SGPA<<" "<<tt.SRANK<<endl;

                    GPAInfo.push_back(tt);
                }
                //上一个专业的gpa已经全部处理完，把sameMajor清空并存入新的学生的gpa
                sameMajor.clear();

            }
            personal.clear();//计算完一个人的gpa，personal清空
            personal.push_back(psl);//然后再存第二个专业第一个人的第一条psl记录到personal中
        }
    }
    //还有最后一个专业的gpa要处理（因为query.next()到最后一条空记录的时候会跳出
    float gpa=0;

    for(vector<cal_gpa>::iterator it=personal.begin();it!=personal.end();it++)
    {
        gpa+=(it->score/100.0)*it->credit;
    }
    tmp.SNO=personal.begin()->SNO;
    tmp.SGPA=gpa;

    personal.clear();//计算完一个人的gpa，personal清空
    personal.push_back(psl);//然后再存本次的psl到personal中

    //在加入到temp之前，先要看看这个tmp学生是不是和上一个学生同一个专业
    //如果是，那么直接把这个学生的gpa存入sameMajor里面
    sameMajor.push_back(tmp);//把同一个专业的学生放到一个vector里面计算排名
    //如果第二个专业的第一条记录已经被读入，那么上一个专业的最后一条记录也已经被存入，可以开始计算排名了
    if(psl.MNAME.trimmed().compare(personal.begin()->MNAME.trimmed())!=0)
    {

        //qDebug()<<psl.MNAME<<" "<<personal.begin()->MNAME<<endl;


        //先对sameMajor的所有学生的gpa进行排序
        sort(sameMajor.begin(),sameMajor.end());//以gpa作为排序标准进行排序
        //排好序以后，给排名赋值，这条记录就完整了，可以直接加入到GPAInfo里面
        int j=1;
        for(vector<GPA>::iterator it=sameMajor.begin();it!=sameMajor.end();it++,j++)
        {
            it->SRANK=j;
            GPA tt;
            tt.SNO=it->SNO;
            tt.SGPA=it->SGPA;
            tt.SRANK=it->SRANK;
            //qDebug()<<tt.SNO<<" "<<tt.SGPA<<" "<<tt.SRANK<<endl;

            GPAInfo.push_back(tt);
        }
        //上一个专业的gpa已经全部处理完，把sameMajor清空并存入新的学生的gpa
        sameMajor.clear();

    }

    //把读到GPAInfo的数据入库
    if(writeInfoToDB_3())
    {
        QMessageBox::information(NULL, "提示", "生成GPA失败，写入GPA表时发生错误，错误类型为："+errorMessage);
    }
    else
        QMessageBox::information(NULL, "提示", "您已成功生成全部学生的gpa！");
}

//清空所有学生的GPA
void make_query_gpa::on_pushButton_2_clicked()
{
    int reply;
    reply = QMessageBox::warning(this, "！重要提示！", "您正在执行高危操作，你确定要删库跑路吗?",
                                 QMessageBox::Yes , QMessageBox::No);
    if(reply==QMessageBox::Yes)
    {
        int reply2;
        reply2 = QMessageBox::question(this, "！重要提示！", "最后确认，你真的要删除系统中关于GPA的所有数据吗?",
                                       QMessageBox::Yes , QMessageBox::No);
        if(reply2==QMessageBox::Yes)
        {
            //开始删除课程表
            QSqlQuery query;
            if(!query.exec("DELETE FROM GPA"))
            {
                QMessageBox::information(this,"提示",
                                         "清空GPA表失败，你看着办吧："+query.lastError().text());
            }
            QMessageBox::information(this,"提示",
                                     "你已经成功删库，现在准备跑路吧，祝您生活愉快~~");
        }
        else return;
    }
    else return;
}



void make_query_gpa::on_pushButton_3_clicked()
{
    int reply;
    reply = QMessageBox::warning(this, "！重要提示！", "您正在清空学生成绩表（SC表），你确定要删库跑路吗?",
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
            if(!query.exec("DELETE FROM SC"))
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

//导出结果为CSV文件
void make_query_gpa::on_pushButton_4_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, "请选择存放CSV的路径", "./" + ui->comboBox->currentText()+ui->lineEdit->text().trimmed()+"GPA查询结果", tr("CSV file (*.csv)"));
    if (!path.isEmpty())
    {
        if(outCSVfromTable(path,ui->tableWidget,"学号,姓名,专业,GPA,专业排名"))
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
