#ifndef QUERYMI_H
#define QUERYMI_H
#include <QMenu>
#include <QWidget>
#include "base.h"
#include "edit_mi_dialog.h"
namespace Ui {
class queryMI;
}

class queryMI : public QWidget
{
    Q_OBJECT

public:
    explicit queryMI(QWidget *parent = 0);
    ~queryMI();

private slots:
    void on_pushButton_clicked();
    void slot_show_result(int errorCode);
    void progressValue2(int pvalue);
    void on_tableWidget_customContextMenuRequested(const QPoint &pos);
    void mi_slot_DL_ActDelete();
    void mi_slot_DL_ActEdit();
    void mi_slot_update_table(MI after_edit);
    void on_pushButton_2_clicked();

private:
    Ui::queryMI *ui;
    QMenu *popMenu;
    QAction *actDelete;
    QAction *actEdit;
    int select_row;
    QString selected_MNAME;
    edit_mi_dialog *EMD;
};

#endif // QUERYMI_H
