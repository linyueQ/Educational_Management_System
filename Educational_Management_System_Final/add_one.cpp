#include "add_one.h"
#include "ui_add_one.h"
#include"base.h"

add_one::add_one(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_one)
{
    ui->setupUi(this);
}

add_one::~add_one()
{
    delete ui;
}

//当管理员选择了用户类型之后，需要根据管理员的选择初始化下面的label下拉框
void add_one::on_comboBox_usertype_currentIndexChanged(int index)
{
    int userType=ui->comboBox_usertype->currentIndex();//获取当前选择的用户类型
    QSqlQuery query;
    switch(userType)
    {
    //学生就填充所有专业的名字,还有年级下拉框
    case 1:
        ui->comboBox_label->clear();//填充之前先清除一下下拉框
        query.clear();
        query.exec("select distinct MNAME from MI");
        while(query.next())
        {
            ui->comboBox_label->addItem(query.value(0).toString().trimmed());
        }

        ui->comboBox_grade->setEnabled(true);
        ui->comboBox_grade->clear();
        query.clear();
        query.exec("select distinct GRADE from GLIST");
        while(query.next())
        {
            ui->comboBox_grade->addItem(query.value(0).toString().trimmed());
        }
        break;
        //老师就填充所有学院的名字
    case 2:
        ui->comboBox_label->clear();//填充之前先清除一下下拉框
        ui->comboBox_grade->clear();
        ui->comboBox_grade->setDisabled(true);//选老师的学院的时候把年级下拉框屏蔽掉
        query.clear();
        query.exec("select distinct INAME from MI");
        while(query.next())
        {
            ui->comboBox_label->addItem(query.value(0).toString().trimmed());
        }
        break;
        //管理员就填充所有学院的名字
    case 3:
        ui->comboBox_label->clear();//填充之前先清除一下下拉框
        ui->comboBox_grade->clear();
        ui->comboBox_grade->setDisabled(true);//选管理员的学院的时候把年级下拉框屏蔽掉
        query.clear();
        query.exec("select distinct INAME from MI");
        while(query.next())
        {
            ui->comboBox_label->addItem(query.value(0).toString().trimmed());
        }
        break;
    }
}

//点击了确定导入单个用户的按钮
void add_one::on_pushButton_okin_clicked()
{
    basic tmp;
    QSqlQuery query;//数据库操作接口
    //首先获取这个用户的输入信息
    tmp.name=ui->lineEdit_name->text();
    if(ui->comboBox_sex->currentIndex()==0)
        tmp.sex="M";
    else
        tmp.sex="F";
    tmp.IDnumber=ui->lineEdit_IDnum->text();
    tmp.password=tmp.IDnumber.right(6);//默认密码是身份证后6位
    tmp.label=ui->comboBox_label->currentText();
    tmp.tel=ui->lineEdit_tel->text();
    QString SNO="";
    int nRat;
    bool hasNO=false;//标记垃圾桶里面是否有可用的学号
    //然后需要构建这位用户的学/工号，首先要判断用户类型
    switch(ui->comboBox_usertype->currentIndex())
    {
    case 1:
    {
        //如果是学生用户的话，学号组成是4位年份+2位学院号+3位专业号+2位编号
        //首先组成学号的前缀
        SNO=ui->comboBox_grade->currentText();
        //然后获取这个专业所在的专业号和学院号
        query.clear();
        query.exec("select distinct MI.MNO from MI where MI.MNAME = '"+ui->comboBox_label->currentText().trimmed()+"'");
        while(query.next())
        {
            qDebug()<<query.value(0).toString()<<endl;
            SNO+=query.value(0).toString().trimmed();
        }
        //然后首先检索一下回收站表，看看有没有之前被删除的学号是可以用的，如果可以用，那么直接用这个学号
        query.clear();
        query.prepare("select * from RECYCLE where DNO like ?");
        query.addBindValue(SNO+"%");
        query.exec();
        hasNO=false;//标记垃圾桶里面是否有可用的学号
        while(query.next())
        {
            //能进到这里说明垃圾桶里面有对应前缀的学号，可以取出来用，默认取结果的第一个
            tmp.ID=query.value(0).toString().trimmed();
            //取得了这个学号别忘记把学个学号从垃圾桶删除
            query.clear();
            query.prepare("delete from RECYCLE where DNO like ?");
            query.addBindValue(tmp.ID);
            query.exec();
            hasNO=true;
            break;
        }
        if(!hasNO)
        {
            /*如果垃圾桶里面没有对应前缀的学号，那么就重新生成一个学号,
             * 由于每个班级的学生的编号从01开始，这时候只需要找出这个专业班级的总人数，
             * 让其+1再和原来的学号连接起来就得到了新的学号*/
            query.clear();
            query.prepare("select count(*) from STUDENT where SNO like ?");
            query.addBindValue(SNO.trimmed()+"%");
            qDebug()<<SNO.trimmed()<<endl;
            query.exec();
            while(query.next())
            {
                qDebug()<<query.value(0).toInt()<<endl;
                if(query.value(0).toInt()+1<10)
                    SNO+="0"+QString::number(query.value(0).toInt()+1,10);
                else if(query.value(0).toInt()+1>=10&&query.value(0).toInt()+1<100)
                    SNO+=QString::number(query.value(0).toInt()+1,10);
                break;
            }
            tmp.ID=SNO;
        }
        //然后到这里，这位学生的学号就写好了，可以弹窗提示用户是否要写入到数据库
        nRat = QMessageBox::question(NULL,"确定是否添加该用户","\n您要添加的【学生】的信息如下：\n学号："+tmp.ID+"\n年级："+tmp.ID.left(4)+"\n姓名："+tmp.name+"\n性别："+tmp.sex+"\n身份证号："+tmp.IDnumber+"\n专业："+tmp.label+"\n联系电话："+tmp.tel,
                                     QMessageBox::Yes, QMessageBox::No);
        //如果用户确定写入这名学生
        if (nRat == QMessageBox::Yes)
        {
            //此处执行添加一名学生的动作
            query.clear();
            query.prepare("insert into STUDENT values(?,?,?,?,?,?,?)");
            query.addBindValue(tmp.ID);
            query.addBindValue(tmp.name);
            query.addBindValue(tmp.sex);
            query.addBindValue(tmp.password);
            query.addBindValue(tmp.IDnumber);
            query.addBindValue(tmp.label);
            query.addBindValue(tmp.tel);
            bool pan=query.exec();
            if(!pan)
            {
                qDebug()<<query.lastError()<<endl;
                errorMessage=query.lastError().text();
                QMessageBox::information(NULL,"错误！","在写入数据库时发生错误，错误信息："+errorMessage);
            }
            else
                QMessageBox::information(NULL,"OK","成功写入该名学生信息，请牢记该学号："+tmp.ID);
        }
        break;
    }
    case 2:
    {
        //如果是老师用户的话，工号组成是1+2位学院代码+3位学院内编号
        //首先组成工号的前缀
        SNO="1";
        //然后获取这个学院的学院号
        query.clear();
        query.exec("select distinct MI.INO from MI where MI.INAME = '"+ui->comboBox_label->currentText().trimmed()+"'");
        while(query.next())
        {
            SNO+=query.value(0).toString().trimmed();//加上学院号
        }
        //然后首先检索一下回收站表，看看有没有之前被删除的工号是可以用的，如果可以用，那么直接用这个工号
        query.clear();
        query.prepare("select * from RECYCLE where DNO like ?");
        query.addBindValue(SNO+"%");
        query.exec();
        hasNO=false;//标记垃圾桶里面是否有可用的工号
        while(query.next())
        {
            //能进到这里说明垃圾桶里面有对应前缀的工号，可以取出来用，默认取结果的第一个
            tmp.ID=query.value(0).toString().trimmed();
            //取得了这个学号别忘记把学个学号从垃圾桶删除
            query.clear();
            query.prepare("delete from RECYCLE where DNO like ?");
            query.addBindValue(tmp.ID);
            query.exec();
            hasNO=true;
            break;
        }
        if(!hasNO)
        {
            /*如果垃圾桶里面没有对应前缀的工号，那么就重新生成一个工号,
             * 由于每个学院的老师的编号从001开始，这时候只需要找出这个学院的老师的总认识，
             * 让其+1再和原来的工号连接起来就得到了新的学号*/
            query.clear();
            query.prepare("select count(*) from TEACHER where TNO like ?");
            query.addBindValue(SNO.trimmed()+"%");
            query.exec();
            while(query.next())
            {
                if(query.value(0).toInt()+1<10)
                    SNO+="00"+QString::number(query.value(0).toInt()+1,10);
                else if(query.value(0).toInt()+1>=10&&query.value(0).toInt()+1<100)
                    SNO+="0"+QString::number(query.value(0).toInt()+1,10);
                else
                    SNO+=QString::number(query.value(0).toInt()+1,10);
                break;
            }
            tmp.ID=SNO;
        }
        //然后到这里，这位老师的学号就写好了，可以弹窗提示用户是否要写入到数据库
        nRat = QMessageBox::question(NULL,"确定是否添加该用户","\n您要添加的【老师】的信息如下：\n工号："+tmp.ID+"\n姓名："+tmp.name+"\n性别："+tmp.sex+"\n身份证号："+tmp.IDnumber+"\n所属学院："+tmp.label+"\n联系电话："+tmp.tel,
                                     QMessageBox::Yes, QMessageBox::No);
        //如果用户确定写入这名老师
        if (nRat == QMessageBox::Yes)
        {
            //此处执行添加一名老师的动作
            query.clear();
            query.prepare("insert into TEACHER values(?,?,?,?,?,?,?)");
            query.addBindValue(tmp.ID);
            query.addBindValue(tmp.name);
            query.addBindValue(tmp.sex);
            query.addBindValue(tmp.password);
            query.addBindValue(tmp.IDnumber);
            query.addBindValue(tmp.label);
            query.addBindValue(tmp.tel);
            bool pan=query.exec();
            if(!pan)
            {
                qDebug()<<query.lastError()<<endl;
                errorMessage=query.lastError().text();
                QMessageBox::information(NULL,"错误！","在写入数据库时发生错误，错误信息："+errorMessage);
            }
            else
                QMessageBox::information(NULL,"OK","成功写入该名老师信息，请牢记该工号："+tmp.ID);
        }
        break;
    }
    case 3:
    {
        //如果是管理员用户的话，工号组成是3+2位学院代码+3位学院内编号
        //首先组成工号的前缀
        SNO="3";
        //然后获取这个学院的学院号
        query.clear();
        query.exec("select distinct MI.INO from MI where MI.INAME = '"+ui->comboBox_label->currentText().trimmed()+"'");
        while(query.next())
        {
            SNO+=query.value(0).toString().trimmed();//加上学院号
        }
        //然后首先检索一下回收站表，看看有没有之前被删除的工号是可以用的，如果可以用，那么直接用这个工号
        query.clear();
        query.prepare("select * from RECYCLE where DNO like ?");
        query.addBindValue(SNO+"%");
        query.exec();
        hasNO=false;//标记垃圾桶里面是否有可用的工号
        while(query.next())
        {
            //能进到这里说明垃圾桶里面有对应前缀的工号，可以取出来用，默认取结果的第一个
            tmp.ID=query.value(0).toString().trimmed();
            //取得了这个学号别忘记把学个学号从垃圾桶删除
            query.clear();
            query.prepare("delete from RECYCLE where DNO like ?");
            query.addBindValue(tmp.ID);
            query.exec();
            hasNO=true;
            break;
        }
        if(!hasNO)
        {
            /*如果垃圾桶里面没有对应前缀的工号，那么就重新生成一个工号,
             * 由于每个学院的管理员的编号从001开始，这时候只需要找出这个学院的管理员的总认识，
             * 让其+1再和原来的工号连接起来就得到了新的学号*/
            query.clear();
            query.prepare("select count(*) from ADMIN where ANO like ?");
            query.addBindValue(SNO.trimmed()+"%");
            query.exec();
            while(query.next())
            {
                if(query.value(0).toInt()+1<10)
                    SNO+="00"+QString::number(query.value(0).toInt()+1,10);
                else if(query.value(0).toInt()+1>=10&&query.value(0).toInt()+1<100)
                    SNO+="0"+QString::number(query.value(0).toInt()+1,10);
                else
                    SNO+=QString::number(query.value(0).toInt()+1,10);
                break;
            }
            tmp.ID=SNO;
        }
        //然后到这里，这位管理员的学号就写好了，可以弹窗提示用户是否要写入到数据库
        nRat = QMessageBox::question(NULL,"确定是否添加该用户","\n您要添加的【管理员】的信息如下：\n工号："+tmp.ID+"\n姓名："+tmp.name+"\n性别："+tmp.sex+"\n身份证号："+tmp.IDnumber+"\n所属学院："+tmp.label+"\n联系电话："+tmp.tel,
                                     QMessageBox::Yes, QMessageBox::No);
        //如果用户确定写入这名管理员
        if (nRat == QMessageBox::Yes)
        {
            //此处执行添加一名管理员的动作
            query.clear();
            query.prepare("insert into ADMIN values(?,?,?,?,?,?,?)");
            query.addBindValue(tmp.ID);
            query.addBindValue(tmp.name);
            query.addBindValue(tmp.sex);
            query.addBindValue(tmp.password);
            query.addBindValue(tmp.IDnumber);
            query.addBindValue(tmp.label);
            query.addBindValue(tmp.tel);
            bool pan=query.exec();
            if(!pan)
            {
                qDebug()<<query.lastError()<<endl;
                errorMessage=query.lastError().text();
                QMessageBox::information(NULL,"错误！","在写入数据库时发生错误，错误信息："+errorMessage);
            }
            else
                QMessageBox::information(NULL,"OK","成功写入该名管理员信息，请牢记该工号："+tmp.ID);
        }
        break;
    }
    }
}
