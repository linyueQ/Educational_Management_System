#include "admin.h"
#include "ui_admin.h"
#include"ui_addonemitable.h"

//首先admin.cpp是主界面，add_one是加入一个学生的界面
//add_all是从csv文件加入学生的界面
Admin::Admin(basic currentUser,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Admin)
{
    ui->setupUi(this);

    //this->setMinimumSize(950,480);
    //this->setMaximumSize(1300,800);
//    QMainWindow w;
//    w.setWindowFlags(w.windowFlags()&~Qt::WindowMaximizeButtonHint&~Qt::WindowMinimizeButtonHint);
//    w.showMaximized();
 //   this->setWindowFlags(this->windowFlags()&~Qt::WindowMaximizeButtonHint&~Qt::WindowMinimizeButtonHint);

    this->currentUser=currentUser;//记录一下当前登录用户的信息
    ui->label_currName->setText("欢迎您，【"+this->currentUser.name.trimmed()+"】。");
    ui->label_currID->setText("您的ID是："+this->currentUser.ID.trimmed());

    ui->treeWidget->setColumnCount(1);//标题只设置一列


    ui->treeWidget->setColumnCount(1);//左边导航树设置成1列
    QStringList header;//设计个树的标题头
    header<<QString("~欢迎使用教务系统~");
    ui->treeWidget->setHeaderLabels(header);

    QList<QTreeWidgetItem *> items;//用来记录整棵树的各个父项目
    QTreeWidgetItem *item_parent;//用来创建树的父母节点的指针，后面用new即可创建出一个对象无需重复申请指针
    QTreeWidgetItem *item_child;//用来创建树的孩子节点的指针，后面用new即可创建出一个对象无需重复申请指针

    //增加一个【用户管理】父亲节点
    item_parent=new QTreeWidgetItem();
    item_parent->setText(0,QString("用户管理"));//设置一项的标签
    items.append(item_parent);//把这一项加入到items里面

    //【用户管理】下面有 增加单个用户，批量导入用户，用户查询修改删除 三个孩子
    item_child=new QTreeWidgetItem();
    item_child->setText(0,QString("添加单个用户"));
    /*为了实现QTreeWidget和QStackWidget的关联，这里在QTreeWidget的item项
     *里面加入对应界面在QStackWidget里面的下标位置
     *这句话表示给当前的页面控件加一个数字index标识，之后的双击事件中，会返回这个index，从而在stackwidget中找到对应index的页面
     *函数原型：void QTreeWidgetItem::setData(int column,int role, const QVariant & value)
     *设置第column列指定角色role的数值,第三个参数指不同列上或者不同的role上增加一个额外的数据*/
    item_child->setData(0,Qt::UserRole+1,QVariant(1));//这里的数字必须要和你在stackwidget容器里面放置的顺序一致，否则双击树节点无法切换到正确页面
    item_parent[0].addChild(item_child);

    item_child=new QTreeWidgetItem();
    item_child->setText(0,QString("批量导入用户"));
    item_child->setData(0,Qt::UserRole+1,QVariant(2));
    item_parent[0].addChild(item_child);

    item_child=new QTreeWidgetItem();
    item_child->setText(0,QString("用户查询、修改和删除"));
    item_child->setData(0,Qt::UserRole+1,QVariant(3));
    item_parent[0].addChild(item_child);

    /*李俊祺*/


    //增加一个【课程管理】父亲节点
    item_parent=new QTreeWidgetItem();
    item_parent->setText(0,QString("课程管理"));
    items.append(item_parent);

    //【课程管理】下面有 导入课程列表，查询删除课程列表 两个孩子
    item_child=new QTreeWidgetItem();
    item_child->setText(0,QString("导入课程列表"));
    /*为了实现QTreeWidget和QStackWidget的关联，这里在QTreeWidget的item项里面加入对应界面在QStackWidget里面的下标位置*/
    /*这句话表示给当前的页面控件加一个数字index标识，之后的双击事件中，会返回这个index，从而在stackwidget中找到对应index的页面*/
    item_child->setData(0,Qt::UserRole+1,QVariant(4));//这里的数字必须要和你在stackwidget容器里面放置的顺序一致，否则双击树节点无法切换到正确页面
    item_parent[0].addChild(item_child);

    item_child=new QTreeWidgetItem();
    item_child->setText(0,QString("查询删除课程列表"));
    item_child->setData(0,Qt::UserRole+1,QVariant(5));
    item_parent[0].addChild(item_child);

    //增加一个【学生课表管理】父亲节点
    item_parent=new QTreeWidgetItem();
    item_parent->setText(0,QString("学生课表管理"));
    items.append(item_parent);

    //【学生课表管理】下面有 增加学生课表，查询清空课程表 两个个孩子
    item_child=new QTreeWidgetItem();
    item_child->setText(0,QString("导入学生课表"));
    item_child->setData(0,Qt::UserRole+1,QVariant(6));//这里的数字必须要和你在stackwidget容器里面放置的顺序一致，否则双击树节点无法切换到正确页面
    item_parent[0].addChild(item_child);

    item_child=new QTreeWidgetItem();
    item_child->setText(0,QString("查询清空学生课表"));
    item_child->setData(0,Qt::UserRole+1,QVariant(7));
    item_parent[0].addChild(item_child);

    //增加一个【学生绩点GPA管理】父亲节点
    item_parent=new QTreeWidgetItem();
    item_parent->setText(0,QString("学生绩点及成绩表管理"));
    items.append(item_parent);

    //【学生绩点GPA管理】下面只有一个 生成查看学生绩点
    item_child=new QTreeWidgetItem();
    item_child->setText(0,QString("生成查看学生绩点GPA,清空学生成绩"));
    item_child->setData(0,Qt::UserRole+1,QVariant(8));
    item_parent[0].addChild(item_child);


    /*梁晓阳*/
    //qaq
    //增加一个【学院管理】父亲节点
    item_parent=new QTreeWidgetItem();
    item_parent->setText(0,QString("学院管理"));//设置一项的标签
    items.append(item_parent);//把这一项加入到items里面

    item_child=new QTreeWidgetItem();
    item_child->setText(0,QString("添加单条学院的记录"));
    item_child->setData(0,Qt::UserRole+1,QVariant(9));//这里的数字必须要和你在stackwidget容器里面放置的顺序一致，否则双击树节点无法切换到正确页面
    item_parent[0].addChild(item_child);

    item_child=new QTreeWidgetItem();
    item_child->setText(0,QString("批量添加学院的记录"));
    item_child->setData(0,Qt::UserRole+1,QVariant(10));
    item_parent[0].addChild(item_child);

    item_child=new QTreeWidgetItem();
    item_child->setText(0,QString("查询，修改，删除"));
    item_child->setData(0,Qt::UserRole+1,QVariant(11));
    item_parent[0].addChild(item_child);


    //增加一个【通知管理】父亲节点
    item_parent=new QTreeWidgetItem();
    item_parent->setText(0,QString("通知管理"));
    items.append(item_parent);

    item_child=new QTreeWidgetItem();
    item_child->setText(0,QString("发送通知"));
    item_child->setData(0,Qt::UserRole+1,QVariant(12));//这里的数字必须要和你在stackwidget容器里面放置的顺序一致，否则双击树节点无法切换到正确页面
    item_parent[0].addChild(item_child);

    item_child=new QTreeWidgetItem();
    item_child->setText(0,QString("查询，删除"));
    item_child->setData(0,Qt::UserRole+1,QVariant(13));
    item_parent[0].addChild(item_child);


    //增加一个【个人信息修改】父亲节点
    item_parent=new QTreeWidgetItem();
    item_parent->setText(0,QString("个人信息修改"));//设置一项的标签
    items.append(item_parent);//把这一项加入到items里面

    item_child=new QTreeWidgetItem();
    item_child->setText(0,QString("修改密码"));
    item_child->setData(0,Qt::UserRole+1,QVariant(14));//这里的数字必须要和你在stackwidget容器里面放置的顺序一致，否则双击树节点无法切换到正确页面
    item_parent[0].addChild(item_child);


    //在stackWidget里面添加所需要的页面，注意与上面的item设置得下标对应好号码
    //欢迎界面，系统开始时候默认显示的界面，它的index为0
    wel=new Welcome(2,this->currentUser);
    ui->stackedWidget->addWidget(wel);
    //添加单个用户 界面，index是1
    a_o=new add_one();
    ui->stackedWidget->addWidget(a_o);
    //批量添加用户 界面，index是2
    a_a=new add_all();
    ui->stackedWidget->addWidget(a_a);

    //用户信息查询 界面，index是3
    q_a=new query_all();
    ui->stackedWidget->addWidget(q_a);

    /*李俊祺*/

    //导入课程列表 界面，index是4
    a_c=new add_course();
    ui->stackedWidget->addWidget(a_c);
    //查询删除课程列表 界面，index是5
    q_d_c=new query_delete_course();
    ui->stackedWidget->addWidget(q_d_c);
    //导入学生课表 界面，index是6
    a_s=new add_schedule();
    ui->stackedWidget->addWidget(a_s);
    //查询清空学生课表 界面，index是7
    q_s=new query_schedule();
    ui->stackedWidget->addWidget(q_s);
    //生成查看学生绩点GPA 界面，index是8
    m_q_g=new make_query_gpa();
    ui->stackedWidget->addWidget(m_q_g);


    /*梁晓阳*/
    //qaq
    //添加单个学院信息 index是9
    mione = new addOneMITable();
    ui->stackedWidget->addWidget(mione);
    //添加全部学院信息 index是10
    miall = new addAllMITable();
    ui->stackedWidget->addWidget(miall);
    //查询、删除、修改学院信息 index是11
    querymi = new queryMI();
    ui->stackedWidget->addWidget(querymi);

    //发送通知 ，index是12
    inform = new inform_Table();
    ui->stackedWidget->addWidget(inform);
    //查询通知 ，index是13
    queryinform = new queryInform();
    ui->stackedWidget->addWidget(queryinform);

    //添加修改个人密码 index是14
    c_pw = new Change_Password(this->currentUser);
    ui->stackedWidget->addWidget(c_pw);

    //最后，把定义好的树形列表items加入到treewidget中
    ui->treeWidget->insertTopLevelItems(0, items);
}

Admin::~Admin()
{
    //需要把创建的子窗口全部删除，因为有可能在子窗口里面开了另外的线程。
    delete wel;
    delete a_o;
    delete a_a;
    delete q_a;

    delete a_c;
    delete q_d_c;
    delete a_s;
    delete q_s;
    delete m_q_g;

    delete mione;
    delete miall;

    delete c_pw;

    delete ui;
}

//树节点的双击响应事件，通过获取所点击的item在构造函数那里设置的index值，取出stackwidget里面相应的页面
void Admin::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    ui->stackedWidget->setCurrentIndex(item->data(0,Qt::UserRole+1).toInt());
    mione->ui->MNAME->clear();
}

//退出登录按钮
void Admin::on_pushButton_clicked()
{
    emit logout();
    this->close();
}
