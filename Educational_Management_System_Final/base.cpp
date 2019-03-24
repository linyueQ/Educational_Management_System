//在这个源文件里面处理所有与数据库交互的东西，类里面只处理前台界面变换与响应
#pragma once
#include"base.h"
using namespace std;

//不同用户类型以不同的身份登陆到数据库
const char *DBStudent="admin";
const char *DBStudentPW="admin";

const char *DBTeacher="admin";
const char *DBTeacherPW="admin";

const char *DBAdmin="admin";
const char *DBAdminPW="admin";

//定义三个数组来保存所有对应类型用户的信息
vector<basic> studentInfo;
vector<basic> teacherInfo;
vector<basic> adminInfo;
QString errorMessage="";//用来存储数据库返回的错误提示
int progress;//用来记录进度条的当前进度
vector<basic> failInfo;//用来记录插入失败的记录（一般都是违反主码定义导致插入失败）

vector<MI> miTableInfo;//用来存储读取的学院和专业表信息

vector<course> courseInfo;
vector<schedule> scheduleInfo;
vector<GPA> GPAInfo;
vector<show_gpa> sGPA;
vector<SC> SCInfo;

//把一个文件夹里面特定类型的文件的名字存到一个二维字符数组里面（by 陈章韶）(不用改)
/*path指路径，filetype是文件类型，比如"*.csv"，组成完整的查找路径 路径+\\+文件类型*/
void showdir(string path, string filetype,string result[1000], int &num)
{
    num = 0;//统计文件个数
    //Win10系统的查找句柄
    intptr_t handle;
    //Win7及以下系统的查找句柄
    //long handle;
    struct _finddata_t fileinfo;//文件信息的结构体
    //这里组装一下查找的完整路径
    string tmp=path+"\\"+filetype;
    const char *filepath=tmp.c_str();
    handle = _findfirst(filepath, &fileinfo);//第一次查找
    if (-1 == handle)
        return;
    //把找到的文件的完整路径加文件名存到数组
    result[num]=path+"\\"+fileinfo.name;
    num++;
    while (!_findnext(handle, &fileinfo))//循环查找其他符合的文件，直到找不到其他的为止
    {
        result[num]=path+"\\"+fileinfo.name;//把找到的文件的完整路径加文件名存到数组
        num++;
    }
    _findclose(handle);//别忘了关闭句柄
    return;
}



//（test version）从文本文件导入学生/老师信息，filepath是文本文件的路径,choice代表读取数据的用户类型，1学生，2老师，3管理员
bool InDatFromCSV(char filepath[300], int choice)
{
    ifstream infile(filepath, ios::in);
    if(!infile)
        return false;
    //因为之前读取的记录已经由用户确认并且保存到数据库里面了，所以这里读取新记录的时候首先得把容器清空
    switch(choice)
    {
    case 1:
        studentInfo.clear();
        break;
    case 2:
        teacherInfo.clear();
        break;
    case 3:
        adminInfo.clear();
        break;
    }
    char s[200];
    infile.getline(s, 200);//先把表头读掉
    basic tmp;
    char chtmp[500];
    int sum=0;
    while(true)
    {
        infile.getline(chtmp, 20, ',');
        tmp.ID=QString(chtmp);
        infile.getline(chtmp, 30, ',');
        tmp.name=QString(chtmp);
        infile.getline(chtmp,2,',');
        tmp.sex=QString(chtmp);
        if(chtmp[0] == '3') { break; }//文件末尾有个sex==3的标记，标志着读到文件末尾读取完成
        infile.getline(chtmp, 30, ',');
        tmp.password=QString(chtmp);
        infile.getline(chtmp, 20, ',');
        tmp.IDnumber=QString(chtmp);
        infile.getline(chtmp, 230, ',');
        tmp.label=QString(chtmp);
        infile.getline(chtmp, 21, '\n');
        tmp.tel=QString(chtmp);
        sum++;
        //根据用户的选择把对应的数据存到对应的容器里面
        switch(choice)
        {
        case 1:
        {
            studentInfo.push_back(tmp);
            double dp=sum*1.0/(37000*1.0)*100.0;

            progress=dp;//计算进度
            break;
        }
        case 2:
        {
            teacherInfo.push_back(tmp);
            double dp=sum*1.0/(4000*1.0)*100.0;

            progress=dp;//计算进度
            break;
        }
        case 3:
        {
            adminInfo.push_back(tmp);
            double dp=sum*1.0/(29*1.0)*100.0;

            progress=dp;//计算进度
            break;
        }
        }
    }
    infile.close();
}


//尝试连接数据库
bool connectDB(QString userName,QString password)
{
    QLibrary *hello_lib = NULL;
    //写清楚库的路径，如果放在当前工程的目录下，路径为./Oracle.so
    hello_lib = new QLibrary("E:\\app\\Elealan\\product\\11.2.0\\dbhome_1\\BIN\\oci.dll");

    //加载动态库
    hello_lib->load();
    if (!hello_lib->isLoaded())
    {
        printf("load Oracle failed!\n");
        // return ;
    }
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QSqlDatabase db = QSqlDatabase::addDatabase("QOCI");//添加驱动
    db.setHostName("localhost");//设置主机名
    db.setDatabaseName("orcl");//设置数据库名
    db.setUserName(userName);//设置用户名
    db.setPassword(password);//设置用户密码
    //发送连接
    if(!db.open())
    {
        qDebug() << db.lastError();
        errorMessage=db.lastError().text();
        return false;
    }
    return true;
}


/*create table student (ID char(20),name char(30),sex char(2),password char(30),IDnumber char(20),major char(230),tel char(21),primary key(ID));*/
/*insert into student values('201400347','夏芷天','0','400347','340323199311098662','机械工程_1','15804054236');*/
//经过用户确认后的用户信息数据可以写入到数据库里面了,choice表示待写入的用户类型信息
int writeInfoToDB(int choice)
{

    QSqlQuery query;//提供操作数据库的接口
    int sum;
    //下面涉及到插入数据，所以首先把插入失败记录的vector清空一下
    failInfo.clear();

    switch(choice)
    {
    //写入学生的数据库
    case 1:
        sum=0;
        for(vector<basic>::iterator it=studentInfo.begin();it!=studentInfo.end();it++)
        {
            query.prepare("insert into student values(?,?,?,?,?,?,?)");
            query.addBindValue(it->ID);
            query.addBindValue(it->name);
            query.addBindValue(it->sex);
            query.addBindValue(it->password);
            query.addBindValue(it->IDnumber);
            query.addBindValue(it->label);
            query.addBindValue(it->tel);
            bool pan=query.exec();
            if(!pan)
            {
                studentInfo[sum].errorMessage=query.lastError().text();//记录当前记录插入错误的数据库返回的错误信息
                failInfo.push_back(studentInfo[sum]);
            }
            query.clear();
            sum++;
            double dp=sum*1.0/(studentInfo.size()*1.0)*100.0;

            progress=dp;//计算进度
        }
        break;
        //写入老师的数据库
    case 2:
        sum=0;
        for(vector<basic>::iterator it=teacherInfo.begin();it!=teacherInfo.end();it++)
        {
            query.prepare("insert into teacher values(?,?,?,?,?,?,?)");
            query.addBindValue(it->ID);
            query.addBindValue(it->name);
            query.addBindValue(it->sex);
            query.addBindValue(it->password);
            query.addBindValue(it->IDnumber);
            query.addBindValue(it->label);
            query.addBindValue(it->tel);
            bool pan=query.exec();
            if(!pan)
            {
                teacherInfo[sum].errorMessage=query.lastError().text();//记录当前记录插入错误的数据库返回的错误信息
                failInfo.push_back(teacherInfo[sum]);
            }
            query.clear();
            sum++;
            double dp=sum*1.0/(teacherInfo.size()*1.0)*100.0;

            progress=dp;//计算进度
        }
        break;
        //写入管理员的数据库
    case 3:
        sum=0;
        for(vector<basic>::iterator it=adminInfo.begin();it!=adminInfo.end();it++)
        {
            query.prepare("insert into admin values(?,?,?,?,?,?,?)");
            query.addBindValue(it->ID);
            query.addBindValue(it->name);
            query.addBindValue(it->sex);
            query.addBindValue(it->password);
            query.addBindValue(it->IDnumber);
            query.addBindValue(it->label);
            query.addBindValue(it->tel);
            bool pan=query.exec();
            if(!pan)
            {
                adminInfo[sum].errorMessage=query.lastError().text();//记录当前记录插入错误的数据库返回的错误信息
                failInfo.push_back(adminInfo[sum]);
            }
            query.clear();
            sum++;
            double dp=sum*1.0/(adminInfo.size()*1.0)*100.0;

            progress=dp;//计算进度
        }
        break;
    }
    return failInfo.size();//最后返回插入失败的记录数，如果全部插入成功，那么返回值为0
}

//输入特定的关键字，查询对应的信息库，把结果保存到vector
int queryAll(int choice,QString keywords)
{
    QSqlQuery query;//提供操作数据库的接口
    //关键字为空，直接查询整个数据库
    if(keywords==NULL||keywords.isEmpty())
    {
        query.clear();
        //根据用户的选择查询对应的数据库
        bool pan;
        switch(choice)
        {
        case 1:
            pan=query.exec("select * from STUDENT");
            studentInfo.clear();
            break;
        case 2:
            pan=query.exec("select * from TEACHER");
            teacherInfo.clear();
            break;
        case 3:
            pan=query.exec("select * from ADMIN");
            adminInfo.clear();
            break;
        }
        if(!pan)
        {
            errorMessage=query.lastError().text();
            return 1;
        }

        basic tmp;
        int sum=0;

        query.last();
        int result_sum=query.at()+1;//获取查询结果的记录条数
        qDebug()<<result_sum<<endl;
        query.first();//移动到开头还没行
        query.previous();//还要往前挪一位，不然会丢失第一条结果
        while(query.next())
        {
            tmp.ID=query.value(0).toString().trimmed();
            tmp.name=query.value(1).toString().trimmed();
            tmp.sex=query.value(2).toString().trimmed();
            tmp.password=query.value(3).toString().trimmed();
            tmp.IDnumber=query.value(4).toString().trimmed();
            tmp.label=query.value(5).toString().trimmed();
            tmp.tel=query.value(6).toString().trimmed();
            sum++;
            double dp=sum*1.0/(result_sum*1.0)*100.0;

            progress=dp;//计算进度

            //根据对应的用户类型把结果放到对应容器里面
            switch(choice)
            {
            case 1:
                studentInfo.push_back(tmp);
                break;
            case 2:
                teacherInfo.push_back(tmp);
                break;
            case 3:
                adminInfo.push_back(tmp);
                break;
            }
        }
        return 0;
    }
    //否则，有关键字，就需要分割关键字,以空格作为分隔符
    else
    {
        QStringList keywordlist=keywords.split(" ",QString::SkipEmptyParts);
        /*
        for(int i=0;i<keywordlist.size();i++)
        {
            qDebug()<<keywordlist[i]<<endl;
        }
        */

        //定义用来组装查询语句的QString
        QString zhu1="";
        QString zhu2="";
        QString bind="";
        switch(choice)
        {
        case 1:
            zhu1="select distinct * from STUDENT where (concat(concat(concat(concat(concat(concat(SNO,SNAME),SSEX),SPSW),SID),MNAME),STEL)) like  ";
            zhu2="  and  (concat(concat(concat(concat(concat(concat(SNO,SNAME),SSEX),SPSW),SID),MNAME),STEL)) like  ";
            studentInfo.clear();
            break;
        case 2:
            zhu1="select distinct * from TEACHER where (concat(concat(concat(concat(concat(concat(TNO,TNAME),TSEX),TPSW),TID),INAME),TTEL)) like  ";
            zhu2=" and (concat(concat(concat(concat(concat(concat(TNO,TNAME),TSEX),TPSW),TID),INAME),TTEL)) like ";
            teacherInfo.clear();
            break;
        case 3:
            zhu1="select distinct * from ADMIN where (concat(concat(concat(concat(concat(concat(ANO,ANAME),ASEX),APSW),AID),INAME),ATEL)) like  ";
            zhu2=" and (concat(concat(concat(concat(concat(concat(ANO,ANAME),ASEX),APSW),AID),INAME),ATEL)) like  ";
            adminInfo.clear();
            break;
        }
        for(int i=0;i<keywordlist.size();i++)
        {
            if(i==0)
            {
                bind=zhu1+"'%"+keywordlist[i]+"%' ";
            }
            else
            {
                bind+=zhu2+"'%"+keywordlist[i]+"%' ";
            }
        }

        if(keywordlist.size()>0)
        {
            qDebug()<<bind<<endl;
            if(!query.exec(bind))
            {
                errorMessage=query.lastError().text();
                return 1;
            }
            //下面开始把查询结果装到容器里面
            basic tmp;
            int sum=0;

            query.last();
            int result_sum=query.at()+1;//获取查询结果的记录条数
            qDebug()<<result_sum<<endl;
            query.first();

            query.previous();

            while(query.next())
            {
                tmp.ID=query.value(0).toString().trimmed();
                tmp.name=query.value(1).toString().trimmed();
                tmp.sex=query.value(2).toString().trimmed();
                tmp.password=query.value(3).toString().trimmed();
                tmp.IDnumber=query.value(4).toString().trimmed();
                tmp.label=query.value(5).toString().trimmed();
                tmp.tel=query.value(6).toString().trimmed();
                sum++;

                double dp=sum*1.0/(result_sum*1.0)*100.0;

                progress=dp;//计算进度
                //根据对应的用户类型把结果放到对应容器里面
                switch(choice)
                {
                case 1:
                    studentInfo.push_back(tmp);
                    break;
                case 2:
                    teacherInfo.push_back(tmp);
                    break;
                case 3:
                    adminInfo.push_back(tmp);
                    break;
                }
            }
        }
        return 0;

    }
}

//把在界面显示的表格里面的内容导出到csv文件
bool outCSVfromTable(QString pathname,QTableWidget *table,QString header)
{
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    string tmp = code->fromUnicode(pathname).data();
    ofstream outfile(tmp.c_str(),ios::out);
    if(!outfile)
    {
        return false;
    }
    else
    {
        int i,j;
        QTextCodec *code = QTextCodec::codecForName("GB2312");
        string tmp = code->fromUnicode(header).data();
        outfile<<tmp<<endl;
        for(i=0;i<table->rowCount();i++)
        {
            for(j=0;j<table->columnCount()-1;j++)
            {
                if(table->item(i,j)!=NULL&&!table->item(i,j)->text().isEmpty())
                {
                    string tmp = code->fromUnicode(table->item(i,j)->text()).data();
                    outfile<<tmp<<",";
                }
            }
            if(table->item(i,j)!=NULL&&!table->item(i,j)->text().isEmpty())
            {
                string tmp = code->fromUnicode(table->item(i,j)->text()).data();
                outfile<<tmp<<endl;
            }
        }
        outfile.close();
        return true;
    }
}


/*梁晓阳*/

vector<inform> informTableInfo;

//从csv里导入学院表
bool InMIDatFromCSV(char filepath[])
{
    ifstream infile(filepath, ios::in);
    if(!infile)
        return false;
    miTableInfo.clear();

    MI tmp;
    char chtmp[230];

    int i = 0;
    int mno=0,ino=0;
    //mno 专业  ino 学院
    int l;
    char iplace = 'A'-1;
    char inames[230];
    while(infile.getline(chtmp,230,'\n'))
    {
        char inos[3], mnos[4];
        inos[2] = '\0';
        mnos[3] = '\0';
        int flag = 0;
        int t = chtmp[0];
        qDebug()<<'\n' << chtmp <<t;
        if(chtmp[0]=='@')
            break;
        else if(chtmp[0]=='*')
        {
            strcpy(inames,chtmp);
            ino += 1;

            inos[0]= (ino/10)+'0';
            inos[1]= (ino%10)+'0';
            iplace  +=1;

            flag = 1;
        }

        else
        {
            mno +=1;
            mnos[0] = (mno/100)%100+'0';
            mnos[1] = (mno/10)%10+'0';
            mnos[2] = (mno%10)+'0';
            tmp.MNO= QString(mnos);
            tmp.MNAME = QString(chtmp);
            tmp.INO = QString(inos);
            tmp.INAME = QString(inames).mid(1);

            tmp.IPLACE = QString(iplace);
            miTableInfo.push_back(tmp);
        }
    }

    infile.close();
}


//确认导入学院专业信息
int writeMIInfoToDB()
{
    QSqlQuery query;//提供操作数据库的接口
    for(vector<MI>::iterator it = miTableInfo.begin();it!=miTableInfo.end();it++)
    {
        query.prepare("insert into mi values(?,?,?,?,?)");
        query.addBindValue(it->MNO);
        query.addBindValue(it->MNAME);
        query.addBindValue(it->INO);
        query.addBindValue(it->INAME);
        query.addBindValue(it->IPLACE);
        bool pan = query.exec();
        if(!pan)
        {
            qDebug()<<query.lastError()<<endl;
            errorMessage=query.lastError().text();
            return 1;
        }
        query.clear();
    }
    return 0;
}


//查询学院专业表
int queryMIAll(QString keywords)
{
    QSqlQuery query;
    if(keywords == NULL||keywords.isEmpty())
    {
        query.clear();
        bool pan;
        pan = query.exec("select * from MI");
        miTableInfo.clear();
        if(!pan)
        {
            errorMessage=query.lastError().text();
            return 1;
        }
        MI tmp;
        int sum = 0;
        query.last();
        int result_sum = query.at();
        query.first();
        while(query.next())
        {
            tmp.MNO= query.value(0).toString().trimmed();
            tmp.MNAME= query.value(1).toString().trimmed();
            tmp.INO= query.value(2).toString().trimmed();
            tmp.INAME= query.value(3).toString().trimmed();
            tmp.IPLACE= query.value(4).toString().trimmed();
            sum++;
            double dq = sum*1.0/(result_sum*1.0)*100.0;
            progress = dq;
            miTableInfo.push_back(tmp);
        }
        return 0 ;
    }
    else
    {
        QStringList keywordlist=keywords.split(" ",QString::SkipEmptyParts);
        for(int i=0;i<keywordlist.size();i++)
        {
            qDebug()<<keywordlist[i]<<endl;
        }
        //定义用来组装查询语句的QString
        QString zhu1="";
        QString zhu2="";
        QString bind="";
        zhu1="select distinct * from MI where (concat(concat(concat(concat(MNO,MNAME),INO),INAME),IPLACE)) like ";
        zhu2=" and (concat(concat(concat(concat(MNO,MNAME),INO),INAME),IPLACE)) like ";
        //zhu1="select distinct * from MI where (concat(concat(concat(concat(concat(concat(MNO,MNAME),INO),INAME),IPLACE),MNAME),STEL)) like ";
        //zhu2=" and (concat(concat(concat(concat(concat(concat(SNO,SNAME),SSEX),SPSW),SID),MNAME),STEL)) like ";
        miTableInfo.clear();
        for(int i=0;i<keywordlist.size();i++)
        {
            if(i==0)
            {
                bind=zhu1+"'%"+keywordlist[i]+"%' ";
            }
            else
            {
                bind+=zhu2+"'%"+keywordlist[i]+"%' ";
            }
        }

        if(keywordlist.size()>0)
        {
            if(!query.exec(bind))
            {
                errorMessage=query.lastError().text();
                return 1;
            }
            MI tmp;
            int sum=0;
            int result_sum = query.at();
            query.first();
            query.previous();
            //获取查询结果的记录条数
            while(query.next())
            {

                tmp.MNO= query.value(0).toString();
                tmp.MNAME= query.value(1).toString();
                tmp.INO= query.value(2).toString();
                tmp.INAME= query.value(3).toString();
                tmp.IPLACE= query.value(4).toString();
                sum++;
                double dp=sum*1.0/(result_sum*1.0)*100.0;

                progress=dp;//计算进度
                miTableInfo.push_back(tmp);
            }
        }
        return 0;
    }
}

//查询通知表
int queryInformAll(QString keywords)
{
    QSqlQuery query;
    query.clear();
    if(keywords == NULL||keywords.isEmpty())
    {
        query.clear();
        bool pan;
        pan = query.exec("select * from INFORM");
        informTableInfo.clear();
        if(!pan)
        {
            errorMessage=query.lastError().text();
            return 1;
        }
        inform tmp;
        int sum = 0;
        query.last();
        int result_sum = query.at();
        query.first();
        query.previous();
        while(query.next())
        {

            //INF_NAME char(230),CONTENT varchar2(2000),ITIME date,MNAME char(230),GRADE char(4),RECEIVER int
            tmp.INF_NAME= query.value(0).toString();
            tmp.CONTENT= query.value(1).toString().trimmed();
            tmp.ITIME=query.value(2).value<QDateTime>().toString("yyyy年MM月dd日 hh:mm:ss");
            tmp.MNAME = query.value(3).toString().trimmed();
            tmp.GRADE = query.value(4).toString().trimmed();
            tmp.RECEIVER= query.value(5).toInt();

            sum++;
            double dq = sum*1.0/(result_sum*1.0)*100.0;
            progress = dq;
            informTableInfo.push_back(tmp);
        }
        return 0 ;
    }
    else
    {
        QStringList keywordlist=keywords.split(" ",QString::SkipEmptyParts);
        for(int i=0;i<keywordlist.size();i++)
        {
            qDebug()<<keywordlist[i]<<endl;
        }
        //定义用来组装查询语句的QString
        QString zhu1="";
        QString zhu2="";
        QString bind="";
        zhu1="select distinct * from INFORM where (concat(concat(concat(INF_NAME,CONTENT),ITIME),RECEIVER)) like ";
        zhu2=" and (concat(concat(concat(INF_NAME,CONTENT),ITIME),RECEIVER)) like ";
        //zhu1="select distinct * from MI where (concat(concat(concat(concat(concat(concat(MNO,MNAME),INO),INAME),IPLACE),MNAME),STEL)) like ";
        //zhu2=" and (concat(concat(concat(concat(concat(concat(SNO,SNAME),SSEX),SPSW),SID),MNAME),STEL)) like ";
        informTableInfo.clear();
        for(int i=0;i<keywordlist.size();i++)
        {
            if(i==0)
            {
                bind=zhu1+"'%"+keywordlist[i]+"%' ";
            }
            else
            {
                bind+=zhu2+"'%"+keywordlist[i]+"%' ";
            }
        }

        if(keywordlist.size()>0)
        {
            if(!query.exec(bind))
            {
                errorMessage=query.lastError().text();
                return 1;
            }
            inform tmp;
            int sum=0;
            int result_sum = query.at();
            query.first();
            query.previous();
            //获取查询结果的记录条数
            while(query.next())
            {
                tmp.INF_NAME= query.value(0).toString();
                tmp.CONTENT= query.value(1).toString().trimmed();
                tmp.ITIME= query.value(2).value<QDateTime>().toString("yyyy年MM月dd日 hh:mm:ss");
                tmp.MNAME = query.value(3).toString().trimmed();
                tmp.GRADE = query.value(4).toString().trimmed();
                tmp.RECEIVER= query.value(5).toInt();
                sum++;
                double dp=sum*1.0/(result_sum*1.0)*100.0;

                progress=dp;//计算进度
                informTableInfo.push_back(tmp);
            }
        }
        return 0;
    }
}

/*李俊祺*/


//构建courseInfo数组，这个函数用来把课程列表csv文件的数据放到courseInfo里面
//然后add_course.cpp里面会根据courseInfo这个文件，把数据显示在tableWidget里面
bool InDatFromCSV2(char filepath[300])
{
    ifstream infile(filepath, ios::in);
    if(!infile) return false;
    //因为之前读取的记录已经由用户确认并且保存到数据库里面了，所以这里读取新记录的时候首先得把容器清空
    courseInfo.clear();
    char s[200];
    infile.getline(s, 200);//先把表头读掉
    course tmp;
    char chtmp[500];
    //ofstream outfile("E:/DESKTOP/1.csv",ios::out);
    while(1)
    {
        infile.getline(chtmp,10,',');
        tmp.CNO=QString(chtmp);
        infile.getline(chtmp,230,',');
        tmp.CNAME=QString(chtmp);
        infile>>tmp.CREDIT;//学分直接读取就可以
        infile.getline(chtmp,1,',');//把double后面的逗号读掉
        if(tmp.CREDIT==-1) break;
        infile>>tmp.FLAG;//必修和选修直接读取就可以
        infile.getline(chtmp,5,'\n');//换行

        courseInfo.push_back(tmp);//把读出来的数据存回去
    }

    infile.close();
    //outfile.close();
    return true;
}

//构建scheduleInfo数组，这个函数用来把课程列表csv文件的数据放到courseInfo里面
//然后add_schedule.cpp里面会根据scheduleInfo这个文件，把数据显示在tableWidget里面
bool InDatFromCSV3(char filepath[300])
{
    ifstream infile(filepath, ios::in);
    if(!infile)
        return false;

    char s[200];
    infile.getline(s, 200);//先把表头读掉
    schedule tmp;
    string chtmp;

    //QMessageBox::information(NULL,"OK","现在开始从csv读取数据"+QString(s));

    //ofstream outfile("E:/DESKTOP/1.csv",ios::out);
    while(getline(infile,chtmp))
    {
        istringstream a(chtmp);//先把串读到istringstream里面，然后再慢慢进行分割
        string f;
        int i=0;
        while(i<8&&getline(a,f,','))//分割并将数据存到tmp里面
        {
            if(i==0) tmp.TDAY=QString::fromStdString(f);
            if(i==1) tmp.TTIME=QString::fromStdString(f);
            if(i==2) tmp.CNO=QString::fromStdString(f);
            if(i==3) tmp.TNO=QString::fromStdString(f);
            if(i==4) tmp.MNAME=QString::fromStdString(f);
            if(i==5) tmp.GRADE=QString::fromStdString(f);
            if(i==6) tmp.TPLACE=QString::fromStdString(f);
            if(i==7) tmp.SWEEK=QString::fromStdString(f);
            i++;
        }

        /*outfile<<tmp.TDAY.toStdString()<<','<<tmp.TTIME.toStdString()<<','<<tmp.CNO.toStdString()
              <<','<<tmp.TNO.toStdString()<<','<<tmp.MNAME.toStdString()<<','<<tmp.GRADE.toStdString()
            <<','<<tmp.TPLACE.toStdString()<<','<<tmp.SWEEK.toStdString()<<'\n';*/

        scheduleInfo.push_back(tmp);//把读出来的数据存回去
    }
    infile.close();
    //outfile.close();
}


/*create table COURSE (CNO char(4),CNAME char(50),CREDIT FLOAT,FLAG INTEGER,primary key(CNO));*/
/*insert into student values('0001','计算机科学与技术',2.5,0);*/
//经过管理员确认后的课程列表信息数据可以写入到数据库里面了
int writeInfoToDB_1()
{

    QSqlQuery query;//提供操作数据库的接口
    for(vector<course>::iterator it=courseInfo.begin();it!=courseInfo.end();it++)
    {
        query.prepare("insert into COURSE values(?,?,?,?)");
        query.addBindValue(it->CNO);
        query.addBindValue(it->CNAME);
        query.addBindValue(it->CREDIT);//我担心double不会转成float类型的
        query.addBindValue(it->FLAG);

        bool pan=query.exec();
        if(!pan)
        {
            qDebug()<<query.lastError()<<endl;
            errorMessage=query.lastError().text();
            return 1;
        }
        query.clear();
    }
    return 0;
}

//经过管理员确认后的课程表可以写入到数据库里面
int writeInfoToDB_2()
{
    QSqlQuery query;//提供操作数据库的接口
    for(vector<schedule>::iterator it=scheduleInfo.begin();it!=scheduleInfo.end();it++)
    {
        query.prepare("insert into TIMETABLE values(?,?,?,?,?,?,?,?)");
        query.addBindValue(it->TDAY);
        query.addBindValue(it->TTIME);
        query.addBindValue(it->CNO);//我担心double不会转成float类型的
        query.addBindValue(it->TNO);
        query.addBindValue(it->MNAME);
        query.addBindValue(it->GRADE);
        query.addBindValue(it->TPLACE);
        query.addBindValue(it->SWEEK);

        bool pan=query.exec();
        if(!pan)
        {
            qDebug()<<query.lastError()<<endl;
            errorMessage=query.lastError().text();
            QMessageBox::information(NULL,"插入数据错误","在插入以下元组的时候发生错误，元组信息如下：\n"+it->TDAY+"\n"+it->TTIME+"\n"+it->CNO+"\n"+it->TNO+"\n"+it->MNAME+"\n"+it->GRADE+"\n"+it->TPLACE+"\n"+it->SWEEK);
            //return 1;
        }
        query.clear();
    }
    return 0;
}

//生成完所有的GPA，就可以把GPAInfo中的数据写入到数据库里面
int writeInfoToDB_3()
{
    QSqlQuery query;//提供操作数据库的接口
    for(vector<GPA>::iterator it=GPAInfo.begin();it!=GPAInfo.end();it++)
    {
        query.prepare("insert into GPA values(?,?,?)");
        query.addBindValue(it->SNO);
        query.addBindValue(it->SGPA);
        query.addBindValue(it->SRANK);//我担心double不会转成float类型的

        bool pan=query.exec();
        if(!pan)
        {
            qDebug()<<query.lastError()<<endl;
            errorMessage=query.lastError().text();
            return 1;
        }
        query.clear();
    }
    return 0;
}

//生成课表的时候顺便要把SC表也生成了，这个函数用来把SC的SNO/CNO入库
int writeInfoToDB_4()
{
    QSqlQuery query;//提供操作数据库的接口
    for(vector<SC>::iterator it=SCInfo.begin();it!=SCInfo.end();it++)
    {
        query.prepare("insert into SC values(?,?,?,?)");
        query.addBindValue(it->SNO);
        query.addBindValue(it->CNO);
        query.addBindValue(it->SCORE);//初始的时候SC表中所有人各科的成绩均为-1分

        bool pan=query.exec();
        if(!pan)
        {
            qDebug()<<query.lastError()<<endl;
            errorMessage=query.lastError().text();
            return 1;
        }
        query.clear();
    }
    return 0;
}

//从数据库中删掉某条课程信息
int delInfoFromDB(QString sel_CNO)
{
    QSqlQuery query;//提供操作数据库的接口
    query.clear();

    //根据用户的选择查询对应的数据库
    //先删Course的
    QString a="DELETE FROM COURSE WHERE CNO='";
    a+=sel_CNO;a+='\'';
    //再删SC表的
    QString b="DELETE FROM SC WHERE CNO='";
    b+=sel_CNO;b+='\'';
    //最后删TIMETABLE的
    QString c="DELETE FROM TIMETABLE WHERE CNO='";
    c+=sel_CNO;c+='\'';

    if(!query.exec(a))
    {
        QMessageBox::information(NULL, "error", "删除"+sel_CNO+"在COURSE表中对应的记录失败，原因是："+query.lastError().text());
        return 0;
    }

    if(!query.exec(b))
    {
        QMessageBox::information(NULL, "error", "删除"+sel_CNO+"在SC表中对应的记录失败，原因是："+query.lastError().text());
        return 0;
    }

    if(!query.exec(c))
    {
        QMessageBox::information(NULL, "error", "删除"+sel_CNO+"在TIMETABLE表中对应的记录失败，原因是："+query.lastError().text());
        return 0;
    }

    return 1;
}


queryCourse(QString keywords)
{
    QSqlQuery query;//提供操作数据库的接口
    //关键字为空，直接查询整个数据库
    if(keywords.isEmpty())
    {
        query.clear();
        //根据用户的选择查询对应的数据库
        bool pan;
        pan=query.exec("select * from COURSE");

        if(!pan)
        {
            errorMessage=query.lastError().text();
            return 1;
        }

        courseInfo.clear();//确保在查询过一次后，courseInfo要清空

        course tmp;
        while(query.next())
        {
            tmp.CNO=query.value(0).toString();
            tmp.CNAME=query.value(1).toString();
            tmp.CREDIT=query.value(2).toString().toFloat();
            tmp.FLAG=query.value(3).toString().toInt();

            //把结果放到course的容器courseInfo里面
            courseInfo.push_back((tmp));
        }
        return 0;
    }
    else
    {
        QStringList keywordlist=keywords.split(" ",QString::SkipEmptyParts);
        for(int i=0;i<keywordlist.size();i++)
        {
            qDebug()<<keywordlist[i]<<endl;
        }
        //定义用来组装查询语句的QString
        QString zhu1="";
        QString zhu2="";
        QString bind="";
        zhu1="select distinct * from COURSE where (concat(concat(concat(CNO,CNAME),CREDIT),FLAG)) like ";
        zhu2=" and (concat(concat(concat(CNO,CNAME),CREDIT),FLAG)) like ";

        courseInfo.clear();//确保在查询过一次后，courseInfo要清空

        for(int i=0;i<keywordlist.size();i++)
        {
            if(i==0)
            {
                bind=zhu1+"'%"+keywordlist[i]+"%' ";
            }
            else
            {
                bind+=zhu2+"'%"+keywordlist[i]+"%' ";
            }
        }

        if(!query.exec(bind))
        {
            errorMessage=query.lastError().text();
            return 1;
        }
        //下面开始把查询结果装到容器里面
        course tmp;

        while(query.next())
        {
            tmp.CNO=query.value(0).toString();
            tmp.CNAME=query.value(1).toString();
            tmp.CREDIT=query.value(2).toString().toFloat();
            tmp.FLAG=query.value(3).toString().toInt();

            //根据对应的用户类型把结果放到对应容器里面
            courseInfo.push_back(tmp);
        }
        return 0;
    }
}

int querySchedule(QString year,QString keywords)
{
    QSqlQuery query;//提供操作数据库的接口
    //关键字为空，则返回错误信息
    if(keywords.isEmpty())
    {
        QMessageBox::information(NULL, "提示", "您输入的专业名为空，请重新输入");
        return 0;
    }
    else//如果关键字不为空，则进行模糊查找
    {
        //定义用来组装查询语句的QString
        QString zhu1="";
        QString bind="";
        zhu1="select TIMETABLE.TDAY,TIMETABLE.TTIME,TIMETABLE.CNO,TIMETABLE.TNO,TIMETABLE.MNAME,TIMETABLE.GRADE,TIMETABLE.TPLACE,TIMETABLE.SWEEK,COURSE.CNAME,TEACHER.TNAME from TIMETABLE,COURSE,TEACHER where TIMETABLE.TNO=TEACHER.TNO and TIMETABLE.CNO=COURSE.CNO and TIMETABLE.GRADE='";
        zhu1+=year+="' AND TIMETABLE.MNAME =";
        scheduleInfo.clear();//确保在查询过一次后，courseInfo要清空
        bind=zhu1+"'"+keywords+"' ";

        if(!query.exec(bind))
        {
            errorMessage=query.lastError().text();
            return 1;
        }
        //下面开始把查询结果装到容器里面
        schedule tmp;

        int i=0;//用来判断是否存在这个专业名
        while(query.next())
        {
            tmp.TDAY=query.value(0).toString().trimmed();
            tmp.TTIME=query.value(1).toString().trimmed();
            tmp.CNO=query.value(2).toString().trimmed();
            tmp.TNO=query.value(3).toString().trimmed();
            tmp.MNAME=query.value(4).toString().trimmed();
            tmp.GRADE=query.value(5).toString().trimmed();
            tmp.TPLACE=query.value(6).toString().trimmed();
            tmp.SWEEK=query.value(7).toString().trimmed();
            tmp.CNAME=query.value(8).toString().trimmed();
            tmp.TNAME=query.value(9).toString().trimmed();
            i++;

            //根据对应的用户类型把结果放到对应容器里面
            scheduleInfo.push_back(tmp);
        }
        if(i==0)
        {
            QMessageBox::information(NULL, "提示", "没有这个专业名喔，请重新输入");
            return 1;
        }
        return 0;
    }
}

int queryGPA(QString year,QString keywords)
{
    QSqlQuery query;//提供操作数据库的接口
    //关键字为空，则返回错误信息
    if(keywords.isEmpty())
    {
        QMessageBox::information(NULL, "提示", "没有这个专业名喔，请重新输入");
        return 0;
    }
    else//如果关键字不为空，则进行模糊查找
    {
        //定义用来组装查询语句的QString
        QString zhu1="";
        zhu1="select GPA.SNO,STUDENT.SNAME,STUDENT.MNAME,SGPA,SRANK from GPA,STUDENT where STUDENT.SNO=GPA.SNO AND SUBSTR(STUDENT.SNO,1,4) = '";
        zhu1+=year;zhu1+="' AND MNAME like";
        zhu1+="'%";zhu1+=keywords;zhu1+="%' ";
        sGPA.clear();//确保在查询过一次后，sGPA要清空

        if(!query.exec(zhu1))
        {
            QMessageBox::information(NULL, "错误", "您输入的专业关键字为空，或查询GPA表时发生错误，错误信息："+query.lastError().text());
            return 1;
        }
        //下面开始把查询结果装到容器里面
        show_gpa tmp;

        while(query.next())
        {
            tmp.SNO=query.value(0).toString().trimmed();
            tmp.SNAME=query.value(1).toString().trimmed();
            tmp.MNAME=query.value(2).toString().trimmed();
            tmp.SGPA=query.value(3).toString().toFloat();
            tmp.SRANK=query.value(4).toString().toInt();

            //根据对应的用户类型把结果放到对应容器里面
            sGPA.push_back(tmp);
        }
        return 0;
    }
}


//该函数用于显示课程表，但在使用前，请务必先把数据库的数据读到scheduleInfo里面
bool showSchedule(QTableWidget* s)
{
    vector<schedule>::iterator begin;//定义遍历器的开始
        vector<schedule>::iterator end;//定义遍历器的结束
        begin=scheduleInfo.begin();
        end=scheduleInfo.end();

        //下面就是格式控制了，控制到底应该把哪一条记录放在哪里
        int i=0,x,y;
        for(vector<schedule>::iterator it=begin;it!=end;it++)
        {
            //先确定行数
            if(it->TTIME.compare(QString("8:50-10:25"))==0) x=0;
            else if(it->TTIME.compare(QString("10:40-12:15"))==0) x=1;
            else if(it->TTIME.compare(QString("14:00-15:35"))==0) x=2;
            else if(it->TTIME.compare(QString("15:45-16:30"))==0) x=3;
            else if(it->TTIME.compare(QString("16:35-17:20"))==0) x=4;
            else if(it->TTIME.compare(QString("19:00-21:30"))==0) x=5;
            else{
                QMessageBox::information(NULL, "错误提示",
                                         "该课程的具体时间段数据（TTIME）有误，无法识别，错误的课程的课程号如下"+it->CNO);
                return false;
            }

            //在确定列数
            if(it->TDAY.compare(QString("星期一"))==0) y=0;
            else if(it->TDAY.compare(QString("星期二"))==0) y=1;
            else if(it->TDAY.compare(QString("星期三"))==0) y=2;
            else if(it->TDAY.compare(QString("星期四"))==0) y=3;
            else if(it->TDAY.compare(QString("星期五"))==0) y=4;
            else if(it->TDAY.compare(QString("星期六"))==0) y=5;
            else if(it->TDAY.compare(QString("星期天"))==0) y=6;
            else {
                QMessageBox::information(NULL, "错误提示",
                                         "该课程的星期几（TDAY)有误，无法识别，错误的课程的课程号如下"+it->CNO);
                return false;
            }

            //确定位置以后就把要显示的参数都扔到该表里面
            //不过在显示之前还必须要处理一下CNO和TNO，获取课程名字和老师名字

            //处理CNO
            QSqlQuery query;
            QString sentence="SELECT CNAME FROM COURSE WHERE CNO LIKE '";
            sentence+=it->CNO.trimmed();  sentence+="%'";

            qDebug()<<sentence<<endl;//测试一下

            if(!query.exec(sentence))
            {
                QMessageBox::information(NULL, "错误", "查询的时候发生错误，错误信息："+query.lastError().text());
                return 0;
            }
            query.next();//移到第一条查询结果的记录
            QString CNAME=query.value(0).toString().trimmed();
            query.clear();

            //处理TNO
            QString sentence2="SELECT TNAME FROM TEACHER WHERE TNO LIKE '";
            sentence2+=it->TNO.trimmed(); sentence2+="%'";

            qDebug()<<sentence2<<endl;//测试一下

            if(!query.exec(sentence2))
            {
                QMessageBox::information(NULL, "错误", "查询的时候发生错误，错误信息："+query.lastError().text());
                return 0;
            }
            query.next();
            QString TNAME=query.value(0).toString().trimmed();

            //我们现在有位置，有CNAME，有TNAME，总算可以把数据扔表里了
            //先构造一个QString吧

            QString show="";
            show+=CNAME;show+="\n";show+=TNAME;show+='\n';show+='(';show+=it->SWEEK.trimmed();
            show+=')';show+='\n';show+=it->TPLACE.trimmed();

            //最后把构造好的串扔到课程表的相应位置就大功告成了
            s->setItem(x,y,new QTableWidgetItem(QString(show)));
            s->item(x,y)->setTextAlignment(Qt::AlignCenter);//居中显示

            i++;//别忘记了还要加自增，一个课表可不止一门课
        }
        return true;
}

/*黄丹仪，老师部分*/

//用于存储教师所教课程的信息（querryallcourse)
vector<teachinf1> teachcourse1;
vector<teachinf2> teachcourse2;

//用于存储教师所教班级（studentnamelist)
vector<teachinf3> teachcourse3;

//用于存储学生名单列表
vector<teachinf4> teachcourse4;
vector<teachinf5> teachcourse5;

int queryallcourse(QString tno)
{

    QSqlQuery query;//提供操作数据库的接口

    if(tno==NULL||tno.isEmpty())
    {
        //弹出一个对话框，提示错误
    }
    query.clear();
    bool pan1;
    QString dan="";
    QString dan1="";
    //QString shi="";
    /*shi="select * from TEACHER where TNO='001'";
    query.exec(shi);
    qDebug()<<query.next();
    query.clear();*/
    dan="select * from TIMETABLE where TNO = ";
    dan1=dan+"'"+tno+"' ";
    pan1=query.exec(dan1);
    teachcourse1.clear();
    if(!pan1)
    {
        errorMessage=query.lastError().text();
        return 1;
    }

    //下面开始把查询结果装到容器里面
    teachinf1 tmp1;
    teachinf2 tmp2;
    while(query.next())
    {

        tmp1.CNO=query.value(2).toString().trimmed();
        tmp1.SWEEK=query.value(7).toString().trimmed();
        tmp1.TIMEE=query.value(0).toString().trimmed()+query.value(1).toString().trimmed();
        tmp1.TPLACE=query.value(6).toString().trimmed();
        tmp1.MNAME=query.value(4).toString().trimmed();
        tmp1.GRADE=query.value(5).toString().trimmed();
        /*qDebug()<<tmp1.CNO<<"  "<<tmp1.SWEEK<<"  "<<tmp1.TIMEE<<"  "<<tmp1.TPLACE;
        qDebug()<<"  "<<tmp1.MNAME<<"  "<<tmp1.GRADE;*/
        //把结果放到对应容器里面
        teachcourse1.push_back(tmp1);
    }
    //遍历tmp1，取出cno在course表查询其他信息
    vector<teachinf1>::iterator begin1;//定义遍历器的开始
    vector<teachinf1>::iterator end1;//定义遍历器的结束

    begin1=teachcourse1.begin();
    end1=teachcourse1.end();



    vector<teachinf1>::iterator it1=begin1;
    teachcourse2.clear();
    for(;it1!=end1;)
    {
        QString cno=QString(it1->CNO);
        //查询
        QString yi="";
        QString yi1="";
        yi="select * from COURSE where CNO = ";
        yi1=yi+"'"+cno+"' ";

        //把结果放到对应容器里面
        if(!query.exec(yi1))
        {
            errorMessage=query.lastError().text();
            return 1;
        }

        while(query.next())
        {
            tmp2.CNAME=query.value(1).toString().trimmed();
            tmp2.FLAG=query.value(3).toString().trimmed();
            tmp2.CREDIT=query.value(2).toString().trimmed();


            //把结果放到对应容器里面
            teachcourse2.push_back(tmp2);
        }
        it1++;


    }
    qDebug()<<teachcourse2.size();
    return 0;

}

//存储某老师所教的班级名称，需要查询年级，专业，课程号，课程名
//要考录某个老师给同一个班级上不同的课的情况
int classnamelist(QString tno)
{
    QSqlQuery query;//提供操作数据库的接口

    if(tno==NULL||tno.isEmpty())
    {
        //弹出一个对话框，提示错误
    }
    query.clear();
    bool pan1;
    QString dan="select distinct TIMETABLE.GRADE,TIMETABLE.MNAME,TIMETABLE.CNO,COURSE.CNAME from TIMETABLE,COURSE where TNO = '"+tno+"' and COURSE.CNO=TIMETABLE.CNO ";
    pan1=query.exec(dan);
    teachcourse3.clear();
    if(!pan1)
    {
        errorMessage=query.lastError().text();
        return 1;
    }

    //下面开始把查询结果撞到容器里面
    teachinf3 tmp3;
    while(query.next())
    {
        tmp3.GRADE=query.value(0).toString().trimmed();
        tmp3.MNAME=query.value(1).toString().trimmed();
        tmp3.CNO=query.value(2).toString().trimmed();
        tmp3.CNAME=query.value(3).toString().trimmed();
        teachcourse3.push_back(tmp3);
        //qDebug()<<tmp3.GRADE<<tmp3.MNAME;

    }
    return 0;

}

//查询某班级的学生名单
int searchstudentlist(QString grade,QString mname,QString CNO)
{
    QSqlQuery query;
    if(grade==NULL||grade.isEmpty()||mname==NULL||mname.isEmpty())
    {
        //输出错误信息
    }
    else{
        query.clear();
        QString dan="select STUDENT.SNO,STUDENT.SNAME,SC.SCORE,SC.SRANK "
                    "from STUDENT,SC"
                    " where STUDENT.SNO like '"+grade+"%' "
                                                      "and STUDENT.MNAME='"+mname+"' "
                                                                                  "and STUDENT.SNO = SC.SNO and SC.CNO= '"+CNO+"'";

        //dan=dan+"select SNO,SNAME from STUDENT where SNO like "+"'"+grade+"%' and MNAME="+"'"+mname+"'";
        bool pan;
        pan=query.exec(dan);
        teachcourse4.clear();
        if(!pan)
        {
            errorMessage=query.lastError().text();
            return 1;
        }

        teachinf4 tmp;
        while(query.next())
        {
            tmp.SNO=query.value(0).toString().trimmed();
            tmp.SNAME=query.value(1).toString().trimmed();
            tmp.score=query.value(2).toFloat();
            tmp.CNO=CNO;
            tmp.srank=query.value(3).toInt();
            teachcourse4.push_back(tmp);

        }
        return 0;
    }

}
