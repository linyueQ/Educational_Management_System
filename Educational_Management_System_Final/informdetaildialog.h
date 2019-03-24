#ifndef INFORMDETAILDIALOG_H
#define INFORMDETAILDIALOG_H
#include "base.h"
#include <QDialog>

namespace Ui {
class informDetailDialog;
}

class informDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit informDetailDialog(QWidget *parent = 0);
    ~informDetailDialog();
    informDetailDialog(inform detail,QWidget *parent = 0);
private:
    Ui::informDetailDialog *ui;
};

#endif // INFORMDETAILDIALOG_H
