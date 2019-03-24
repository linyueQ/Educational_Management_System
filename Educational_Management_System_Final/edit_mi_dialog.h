#ifndef EDIT_MI_DIALOG_H
#define EDIT_MI_DIALOG_H

#include <QDialog>
#include"base.h"
namespace Ui {
class edit_mi_dialog;
}

class edit_mi_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit edit_mi_dialog(QWidget *parent = 0);
    ~edit_mi_dialog();
    edit_mi_dialog(MI edit_mi,QWidget *parent = 0);
    void accept();
signals:
    void sendNewMI(MI newMI);
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::edit_mi_dialog *ui;
    MI after_edit;
};

#endif // EDIT_MI_DIALOG_H
