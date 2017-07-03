#include "maximumfilterdialog.h"
#include "ui_maximumfilterdialog.h"

MaximumFilterDialog::MaximumFilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MaximumFilterDialog)
{
    ui->setupUi(this);
}

MaximumFilterDialog::~MaximumFilterDialog()
{
    delete ui;
}

void MaximumFilterDialog::on_buttonBox_accepted()
{
    bool ok;
    int size = ui->lineEdit->text().toInt(&ok);
    bool gray =ui->radioButton_Gray->isChecked();
    if (ok)
        emit confirmed(size,gray);
}
