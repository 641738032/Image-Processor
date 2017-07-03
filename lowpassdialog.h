#ifndef LOWPASSDIALOG_H
#define LOWPASSDIALOG_H

#include <QDialog>
#include<matrix.h>
namespace Ui {
class LowPassDialog;
}

class LowPassDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LowPassDialog(QWidget *parent = 0);
    ~LowPassDialog();

private:
    Ui::LowPassDialog *ui;

public slots:
    void on_buttonBox_accepted();
signals:
    void confirmed(Matrix<double>& mat,bool);
};

#endif // LOWPASSDIALOG_H
