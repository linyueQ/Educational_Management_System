#include "teacher.h"
#include "ui_teacher.h"

Teacher::Teacher(basic currentTeacher,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Teacher)
{
    ui->setupUi(this);
    this->setMinimumSize(950,480);
    this->setMaximumSize(1300,800);

    this->currentUser=currentTeacher;
    ui->label_currName->setText(QString(u8"欢迎您，【")+this->currentUser.name.trimmed().toUtf8().constData()+u8"】。");
    qDebug()<<this->currentUser.name.trimmed();
    ui->label_currID->setText("您的ID是："+this->currentUser.ID.trimmed());

    ui->treeWidget->setColumnCount(1);//标题只设置一列

    QStringList header;//设计个树的标题头
    header<<QString("~欢迎使用教务系统~");
    ui->treeWidget->setHeaderLabels(header);

    QList<QTreeWidgetItem *> items;//用来记录整棵树的各个父项目
    QTreeWidgetItem *item_parent;//用来创建树的父母节点的指针，后面用new即可创建出一个对象无需重复申请指针
    QTreeWidgetItem *item_child;//用来创建树的孩子节点的指针，后面用new即可创建出一个对象无需重复申请指针

    //增加一个【教师专用】父亲节点
    item_parent=new QTreeWidgetItem();
    item_parent->setText(0,QString("教师专用"));//设置一项的标签
    items.append(item_parent);//把这一项加入到items里面

    //【教师专用】下面有 查询教授课程信息、学生名单，成绩导入3个孩纸
    item_child=new QTreeWidgetItem();
    item_child->setText(0,QString("查询教授课程信息"));
    item_child->setData(0,Qt::UserRole+1,QVariant(1));//这里的数字必须要和你在stackwidget容器里面放置的顺序一致，否则双击树节点无法切换到正确页面
    item_parent[0].addChild(item_child);

    item_child=new QTreeWidgetItem();
    item_child->setText(0,QString("学生名单"));
    item_child->setData(0,Qt::UserRole+1,QVariant(2));
    item_parent[0].addChild(item_child);

    item_child=new QTreeWidgetItem();
    item_child->setText(0,QString("学生成绩录入、查询和修改"));
    item_child->setData(0,Qt::UserRole+1,QVariant(3));
    item_parent[0].addChild(item_child);

    item_child=new QTreeWidgetItem();
    item_child->setText(0,QString("修改个人密码"));
    item_child->setData(0,Qt::UserRole+1,QVariant(4));
    item_parent[0].addChild(item_child);

    item_child=new QTreeWidgetItem();
    item_child->setText(0,QString("发送通知"));
    item_child->setData(0,Qt::UserRole+1,QVariant(5));
    item_parent[0].addChild(item_child);


    wel=new Welcome(1,this->currentUser);
    ui->stackedWidget->addWidget(wel);
    //查询教授课程信息 界面，index是1
    a_1=new teachersearch(this->currentUser);
    ui->stackedWidget->addWidget(a_1);
    //学生名单 界面，index是2
    a_2=new studentlist(this->currentUser);
    ui->stackedWidget->addWidget(a_2);

    //学生成绩管理 界面，index是3
    s_s=new student_score(this->currentUser);
    ui->stackedWidget->addWidget(s_s);

    //修改密码界面，index是4
    c_p=new Change_Password(this->currentUser);
    ui->stackedWidget->addWidget(c_p);

    //发送通知界面，index是5
    a_3=new Inform(this->currentUser);
    ui->stackedWidget->addWidget(a_3);


    //最后，把定义好的树形列表items加入到treewidget中
    ui->treeWidget->insertTopLevelItems(0, items);

}

Teacher::~Teacher()
{
    delete wel;
    delete a_1;
    delete a_2;
    delete s_s;
    delete c_p;
    delete a_3;

    delete ui;
}

//树节点的双击响应事件，通过获取所点击的item在构造函数那里设置的index值，取出stackwidget里面相应的页面
void Teacher::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    ui->stackedWidget->setCurrentIndex(item->data(0,Qt::UserRole+1).toInt());
}

//退出登录按钮
void Teacher::on_pushButton_clicked()
{
    emit logout();
    this->close();
}
