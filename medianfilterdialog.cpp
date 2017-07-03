#include "medianfilterdialog.h"
#include "ui_medianfilterdialog.h"

MedianFilterDialog::MedianFilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MedianFilterDialog)
{
    ui->setupUi(this);
}

MedianFilterDialog::~MedianFilterDialog()
{
    delete ui;
}

void MedianFilterDialog::on_buttonBox_accepted()
{
    bool ok;
    int size = ui->lineEdit->text().toInt(&ok);
    bool gray = ui->radiobtn_Gray->isChecked();
    if (ok)
        emit confirmed(size,gray);
}
