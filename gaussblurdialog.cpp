#include "gaussblurdialog.h"
#include "ui_gaussblurdialog.h"

GaussBlurDialog::GaussBlurDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GaussBlurDialog)
{
    ui->setupUi(this);
}

GaussBlurDialog::~GaussBlurDialog()
{
    delete ui;
}
void GaussBlurDialog::on_buttonBox_accepted()
{
    bool ok;
    int size = ui->lineEdit1->text().toInt(&ok);
    double sigma = ui->lineEdit2->text().toDouble(&ok);
    bool gray = ui->radiobtn_Gary->isChecked();
    if (ok)
        emit confirmed(size,sigma,gray);
}
