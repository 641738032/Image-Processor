#ifndef TRANSFORMDIALOG_H
#define TRANSFORMDIALOG_H

#include <QDialog>
#include<QMatrix>
namespace Ui {
class TransformDialog;
}

class TransformDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransformDialog(QWidget *parent = 0);
    ~TransformDialog();

private:
    Ui::TransformDialog *ui;

public slots:
    void on_buttonBox_accepted();

signals:
    void confirmed(const QMatrix& mat);
};

#endif // TRANSFORMDIALOG_H
