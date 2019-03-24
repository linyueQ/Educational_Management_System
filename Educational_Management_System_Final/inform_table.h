#ifndef INFORM_TABLE_H
#define INFORM_TABLE_H

#include <QWidget>

namespace Ui {
class inform_Table;
}

class inform_Table : public QWidget
{
    Q_OBJECT

public:
    explicit inform_Table(QWidget *parent = 0);
    ~inform_Table();

private slots:
    void on_userType_currentIndexChanged(int index);

    void on_pushButton_okin_clicked();

    void on_inameType_currentTextChanged(const QString &arg1);

private:
    Ui::inform_Table *ui;
};

#endif // INFORM_TABLE_H
