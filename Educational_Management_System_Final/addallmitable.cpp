#include "addallmitable.h"
#include "ui_addallmitable.h"

addAllMITable::addAllMITable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addAllMITable)
{
    ui->setupUi(this);
    ui->lineEdit->setText("请点击右边的按钮选择文件导入...");
    ui->tableWidget->clear();
}

addAllMITable::~addAllMITable()
{
    delete ui;
}


void addAllMITable::on_pushButton_2_clicked()
{
    int errorcode = writeMIInfoToDB();
    if(errorcode == 0)
        QMessageBox::information(this,"OK","Insert the data successfully!");
    else if(errorcode==1)
        QMessageBox::information(this,"Fail","Failed to write information into DB!，the error message:"+errorMessage);
}



void addAllMITable::on_pushButton_clicked()
{

    ui->tableWidget->clear();
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle("请选择需要导入的CSV文件");
    fileDialog->setDirectory(".");
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setViewMode(QFileDialog::Detail);//详细
    fileDialog->setDefaultSuffix("*.csv");
    //判断是否选择了文件
    if (fileDialog->exec() == QDialog::Accepted)
    {
        QString path = fileDialog->selectedFiles()[0];
        QMessageBox::information(NULL, "测试", "你选择了 "+path);
        QTextCodec *code = QTextCodec::codecForName("GB2312");
        string tmp = code->fromUnicode(path).data();
        char filepath[300];
        strcpy(filepath, tmp.c_str());
        //int uesrType=ui->comboBox->currentIndex()+1;//先确定用户选择的用户类型号码
        if(InMIDatFromCSV(filepath))
        {
            ui->lineEdit->setText(path);
            ui->tableWidget->clear();//每次使用表格前先清除上次留下来的内容
            ui->tableWidget->setColumnCount(5); //设置列数

            //如果数据导入成功，则展示到表格上面
            QStringList header;//设置表头内容
            vector<MI>::iterator begin;//定义遍历器的开始
            vector<MI>::iterator end;//定义遍历器的结束
            header<<"MNO"<<"MNAME"<<"INO"<<"INAME"<<"IPLACE";
            begin=miTableInfo.begin();
            end=miTableInfo.end();
            ui->tableWidget->setRowCount(miTableInfo.size());//设置行数
            ui->tableWidget->setHorizontalHeaderLabels(header);
            int i=0;
            //设置表格内容
            for(vector<MI>::iterator it=begin;it!=end;it++)
            {
                ui->tableWidget->setItem(i,0,new QTableWidgetItem(it->MNO));
                ui->tableWidget->setItem(i,1,new QTableWidgetItem(it->MNAME));
                ui->tableWidget->setItem(i,2,new QTableWidgetItem(it->INO));
                ui->tableWidget->setItem(i,3,new QTableWidgetItem(it->INAME));
                ui->tableWidget->setItem(i,4,new QTableWidgetItem(it->IPLACE));
                i++;
            }
            ui->tableWidget->repaint();//让表格刷新显示
        }
        else
        {
            QMessageBox::information(NULL, "错误", "在导入如下文件的时候发生错误！无法打开文件"+path);
            ui->lineEdit->setText("请点击右边的按钮选择文件导入...");
        }
    }
    else
    {
        QMessageBox::information(NULL, "错误", "你未选择任何文件，请重新选择！");
        ui->lineEdit->setText("请点击右边的按钮选择文件导入...");
    }

}
