#ifndef MAXIMUMFILTERDIALOG_H
#define MAXIMUMFILTERDIALOG_H

#include <QDialog>

namespace Ui {
class MaximumFilterDialog;
}

class MaximumFilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MaximumFilterDialog(QWidget *parent = 0);
    ~MaximumFilterDialog();

private:
    Ui::MaximumFilterDialog *ui;

private slots:
    void on_buttonBox_accepted();

signals:
    void confirmed(int,bool);
};

#endif // MAXIMUMFILTERDIALOG_H
