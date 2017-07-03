#include "lineardialog.h"
#include "ui_lineardialog.h"

LinearDialog::LinearDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LinearDialog)
{
    ui->setupUi(this);
}

LinearDialog::~LinearDialog()
{
    delete ui;
}

void LinearDialog::on_buttonBox_accepted()
{
    cout<<"buttonBox_accepted"<<endl;
    QString str = ui->textEdit->toPlainText();
    bool gray = ui->radiobtn_Gray->isChecked();
    emit confirmed(str,gray);
}

