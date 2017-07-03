#include "minimumdialog.h"
#include "ui_minimumdialog.h"

MinimumDialog::MinimumDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MinimumDialog)
{
    ui->setupUi(this);
}

MinimumDialog::~MinimumDialog()
{
    delete ui;
}

void MinimumDialog::on_buttonBox_accepted()
{
    bool ok;
    int size = ui->lineEdit->text().toInt(&ok);
    bool gray = ui->radioButton_Gray->isChecked();
    if (ok)
        emit confirmed(size,gray);
}
