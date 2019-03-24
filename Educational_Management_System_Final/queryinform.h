#ifndef QUERYINFORM_H
#define QUERYINFORM_H
#include <QMenu>
#include <QWidget>
#include "base.h"
#include "informdetaildialog.h"
namespace Ui {
class queryInform;
}

class queryInform : public QWidget
{
    Q_OBJECT

public:
    explicit queryInform(QWidget *parent = 0);
    ~queryInform();
private slots:
    void slot_show_result(int errorCode);
    void progressValue2(int pvalue);
    void on_tableWidget_customContextMenuRequested(const QPoint &pos);
    void in_slot_DL_ActDelete();
    void in_slot_DL_ActDetail();
    void on_pushButton_clicked();
    //void sendDetail(inform informDetail);
    void on_pushButton_2_clicked();

signals:
    //sendDetail(inform informDetail);
private:
    Ui::queryInform *ui;
    QMenu *popMenu;
    QAction *actDelete;
    QAction *actDetail;
    int select_row;
    QString selected_INFORM;
    inform detail;
    informDetailDialog *IDD;
};

#endif // QUERYINFORM_H
