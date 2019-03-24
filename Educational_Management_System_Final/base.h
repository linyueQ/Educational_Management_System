/*这个头文件用来存放各种全局的结构和基本功能*/
//ifndef/define/endif主要目的是防止头文件的重复包含和编译
#ifndef BASE_H
#define BASE_H


#pragma once

#pragma once
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<algorithm>
#include<io.h>
#include<direct.h>
#include<time.h>
#include<cstring>
#include<string>
#include<QString>
#include<QMenu>
#include<QAction>
#include<QTableWidget>
#include<QTableWidgetItem>
#include<QtSql/QSqlDatabase>
#include<QtSql>
#include<QDebug>
#include<QHeaderView>
#include<QMessageBox>
#include<QTextCodec>
#include<QWidget>
#include<QFileDialog>
#include<QByteArray>
#include<QMainWindow>
#include<QTreeView>
#include<QTreeWidget>
#include<QTreeWidgetItem>
#include<QStackedWidget>
#include<QPushButton>
#include<QAbstractButton>

using namespace std;

//不同用户类型以不同的身份登陆到数据库
extern const char *DBStudent;
extern const char *DBStudentPW;

extern const char *DBTeacher;
extern const char *DBTeacherPW;

extern const char *DBAdmin;
extern const char *DBAdminPW;


//各种用户的数据库结构定义
struct basic
{
    QString ID;//学号or工号
    QString name;//姓名
    QString sex;//性别，F为女生Female，M为男生Male
    QString password;//登录密码，最大30位字符
    QString IDnumber;//身份证或者其它证件号码，中国大陆身份证18位数
    QString label;//标签，如果是学生就填专业，如果是老师或者管理员（领导）就填所教科目
    QString tel;//联系电话，填手机
    QString errorMessage;//在插入失败的时候，记录下这条记录插入失败的原因
};

//用来存储读取的学院和专业表信息
struct MI
{
    QString MNO;//专业号
        QString MNAME;//专业名
        QString INO;//学院号
        QString INAME;//学院名称
        QString IPLACE;//办公地址
};



struct course//asdf
{
    QString CNO;//课程号
    QString CNAME;//课程名
    float CREDIT;//学分
    int FLAG;//0为必修，1为选修
};

struct schedule//asdf
{
    QString TDAY;//星期几
    QString TTIME;//具体时间段（eg. 8:50-10:25）
    QString CNO;//课程号
    QString TNO;//老师工号
    QString MNAME;//专业名
    QString GRADE;//年级
    QString TPLACE;//上课地点
    QString SWEEK;//上课周数
    QString CNAME;//课程名字
    QString TNAME;//老师名字
};

//这个结构用在计算gpa时暂时存放同一个学生的所有科目的成绩
struct cal_gpa
{
    QString SNO;
    QString CNO;
    QString MNAME;
    float score;
    float credit;
};

struct GPA
{
    QString SNO;
    float SGPA;
    int SRANK;

    //这里重载运算符，后面可以直接用sort对结构体进行降序排列
    bool operator<(const GPA y)const
    {
        return SGPA > y.SGPA;
    }
};

struct SC
{
    QString SNO;
    QString CNO;
    QString CNAME;//课程名
    float CREDIT;//学分数
    float SCORE;
    int SRANK;

    //这里重载运算符，后面可以直接用sort对结构体进行降序排列
    bool operator<(const SC y)const
    {
        return SCORE > y.SCORE;
    }
};

//这个结构用来记录在gpa中要显示的数据
//比SC表多出了一个MNAME的属性，可以用在创建SC表中
struct show_gpa
{
    QString SNO;
    QString SNAME;
    QString MNAME;
    float SGPA;
    int SRANK;
};

extern vector<course> courseInfo;
extern vector<schedule> scheduleInfo;
extern vector<GPA> GPAInfo;
extern vector<show_gpa> sGPA;
extern vector<SC> SCInfo;//仅仅是暂时借用cal_gpa结构里面的SNO和CNO来存放数据

//只声明，不定义
extern vector<basic> studentInfo;
extern vector<basic> teacherInfo;
extern vector<basic> adminInfo;
extern QString errorMessage;//用来存储数据库返回的错误提示
extern int progress;//用来记录进度条的当前进度
extern vector<basic> failInfo;//用来记录插入失败的记录（一般都是违反主码定义导致插入失败）
extern vector<MI> miTableInfo;//用来存储读取的学院和专业表信息

//把一个文件夹里面特定类型的文件的名字存到一个二维字符数组里面（by 陈章韶）(不用改)
/*path指路径，filetype是文件类型，比如"*.csv"，组成完整的查找路径 路径+\\+文件类型*/
void showdir(string path, string filetype,string result[1000], int &num);

//从文本文件导入学生/老师信息，filepath是文本文件的路径
bool InDatFromCSV(char filepath[300], int choice);

//尝试连接数据库
bool connectDB(QString userName,QString password);

//经过用户确认后的用户信息数据可以写入到数据库里面了,choice表示待写入的用户类型信息
int writeInfoToDB(int choice);

//输入特定的关键字，查询对应的信息库，把结果保存到vector
int queryAll(int choice,QString keywords);

//把在界面显示的表格里面的内容导出到csv文件
bool outCSVfromTable(QString pathname,QTableWidget *table,QString header);


/*梁晓阳*/
//用来记录通知表
struct inform
{
    QString INF_NAME;
    QString CONTENT;
    QString ITIME;
    int RECEIVER;
    QString MNAME;
    QString GRADE;

};

extern vector<inform> informTableInfo;

//从文本文件导入学院表信息，filepath是文本的路径
bool InMIDatFromCSV(char filepath[300]);

//把学院表写入数据库
int writeMIInfoToDB();

//输入特定的关键字，查询学院专业的信息库，把结果保存到vector
int queryMIAll(QString keywords);

//查询所有通知表
int queryInformAll(QString keywords);


/*李俊祺*/

//从csv文件导入课程列表信息，filepath是csv文件的路径
bool InDatFromCSV2(char filepath[300]);

//从csv文件导入学生课表信息，filepath是csv文件的路径
bool InDatFromCSV3(char filepath[300]);

//经过管理员确定后的课程信息数据可以写到数据库里面
int writeInfoToDB_1();

//经过管理员确认后的课程表可以写入到数据库里面
int writeInfoToDB_2();

//生成完所有的GPA，就可以把GPAInfo中的数据写入到数据库里面
int writeInfoToDB_3();

//生成课表的时候顺便要把SC表也生成了，这个函数用来把SC的SNO/CNO入库
int writeInfoToDB_4();

//经过管理员确定后，该条课程信息被删除
int delInfoFromDB(QString sel_CNO);


//输入特定的关键字，查询课程信息库，把结果保存到vector
int queryCourse(QString keywords);

//在下拉框选择年份，然后输入特定的关键字，查询课程表信息库，把结果保存到vector
int querySchedule(QString year,QString keywords);

//在下拉框选择年份，然后输入特定的关键字，查询GPA表信息库，把结果保存到vector
int queryGPA(QString year,QString keywords);

//把相应的课程表显示到tableWidget上
bool showSchedule(QTableWidget* s);


/*黄丹仪，老师部分*/


//老师所教课程信息表中结构的定义
struct teachinf1
{
    QString CNO;
    QString SWEEK;
    QString TIMEE;//星期几+具体时间
    QString TPLACE;
    QString GRADE;//年级
    QString MNAME;//专业名

};
extern vector<teachinf1> teachcourse1;


struct teachinf2
{
    QString CNAME;
    QString FLAG;//课程性质
    QString CREDIT;//学分

};
extern vector<teachinf2> teachcourse2;


struct teachinf3
{
    QString GRADE;//年级
    QString MNAME;//专业名
    QString CNO;//课程号
    QString CNAME;//课程名
};
extern vector<teachinf3> teachcourse3;

//这个结构用来记录读取出来的SC表，老师录入成绩的时候将更新SC表
struct teachinf4
{
    QString SNO;//学号
    QString SNAME;//姓名
    QString CNO;//课程号
    float score;//成绩
    int srank;//排名

};
extern vector<teachinf4> teachcourse4;

//这个结构用来记录通知的信息
struct teachinf5
{
    QString INF_NAME;//
    QString CONTENT;//
    QString ITIME;//
    QString MNAME;//
    QString GRADE;
};
extern vector<teachinf5> teachcourse5;


//查询老师所教课程的信息
int queryallcourse(QString tno);
int classnamelist(QString tno);
int searchstudentlist(QString grade,QString mname,QString CNO);

#endif // BASE_H
