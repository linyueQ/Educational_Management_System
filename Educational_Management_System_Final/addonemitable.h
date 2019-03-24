#ifndef ADDONEMITABLE_H
#define ADDONEMITABLE_H

#include <QWidget>

namespace Ui {
class addOneMITable;
}

class addOneMITable : public QWidget
{
    Q_OBJECT

public:
    explicit addOneMITable(QWidget *parent = 0);
    ~addOneMITable();
    void init();
    Ui::addOneMITable *ui;
private slots:
    void on_pushButton_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

private:

};

#endif // ADDONEMITABLE_H
