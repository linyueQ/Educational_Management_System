#ifndef QUERY_ALL_H
#define QUERY_ALL_H

#include <QWidget>
#include"base.h"
#include"edit_user_dialog.h"

namespace Ui {
class query_all;
}

class query_all : public QWidget
{
    Q_OBJECT

public:
    explicit query_all(QWidget *parent = 0);
    ~query_all();

private slots:
    void on_pushButton_clicked();

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void slot_DL_ActDelete();
    void slot_DL_ActEdit();
    void slot_update_table(basic after_edit);

    void slot_show_result(int errorCode,int userType);

    //更新进度条的槽函数，从进度条thread获取实时的进度值并更新进度条
    void progressValue2(int pvalue);

    void on_pushButton_2_clicked();

private:
    Ui::query_all *ui;
    QMenu *popMenu;
    QAction *actDelete;
    QAction *actEdit;
    Edit_User_Dialog *EUD;//修改用户信息的对话框
    QString selected_NO;//获取当前右键单击选中的行的学号/工号，以便在数据库中找到对应的记录进行删除操作
    int select_row;
};

#endif // QUERY_ALL_H
