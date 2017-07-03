#include "bilateralfilterdialog.h"
#include "ui_bilateralfilterdialog.h"

BilateralFilterDialog::BilateralFilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BilateralFilterDialog)
{
    ui->setupUi(this);
}

BilateralFilterDialog::~BilateralFilterDialog()
{
    delete ui;
}

void BilateralFilterDialog::on_buttonBox_accepted()
{
    bool ok;
    int size = ui->lineEdit_size->text().toInt(&ok);
    double sigmaColor = ui->lineEdit_sigmaColor->text().toDouble(&ok);
    double sigmaSpace = ui->lineEdit_sigmaSpace->text().toDouble(&ok);
    if (ok)
        emit confirmed(size,sigmaColor,sigmaSpace);
}
