#ifndef HIGHPASSDIALOG_H
#define HIGHPASSDIALOG_H

#include <QDialog>
#include<matrix.h>
namespace Ui {
class HighPassDialog;
}

class HighPassDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HighPassDialog(QWidget *parent = 0);
    ~HighPassDialog();

private:
    Ui::HighPassDialog *ui;
public slots:
    void on_buttonBox_accepted();
signals:
    void confirmed(Matrix<double>& mat,bool);
};

#endif // HIGHPASSDIALOG_H
