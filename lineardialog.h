#ifndef LINEARDIALOG_H
#define LINEARDIALOG_H

#include <QDialog>
#include<iostream>
using namespace std;

namespace Ui {
class LinearDialog;
}

class LinearDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LinearDialog(QWidget *parent = 0);
    ~LinearDialog();

private:
    Ui::LinearDialog *ui;

public slots:

    void on_buttonBox_accepted();

signals:
    void confirmed(QString,bool);
};

#endif // LINEARDIALOG_H
