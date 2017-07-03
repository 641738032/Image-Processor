#ifndef MINIMUMDIALOG_H
#define MINIMUMDIALOG_H

#include <QDialog>

namespace Ui {
class MinimumDialog;
}

class MinimumDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MinimumDialog(QWidget *parent = 0);
    ~MinimumDialog();

private:
    Ui::MinimumDialog *ui;

private slots:
    void on_buttonBox_accepted();

signals:
    void confirmed(int,bool);
};

#endif // MINIMUMDIALOG_H
