#include "lowpassdialog.h"
#include "ui_lowpassdialog.h"
#include<QDebug>
#include<iostream>
using namespace std;

LowPassDialog::LowPassDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LowPassDialog)
{
    ui->setupUi(this);
}

LowPassDialog::~LowPassDialog()
{
    delete ui;
}

void LowPassDialog::on_buttonBox_accepted()
{
    Matrix<double> mat(3,3,0);
    bool ok;
    mat(0,0) = ui->lineEdit11->text().toDouble(&ok);
    if(!ok) mat(0,0)=0;
    mat(0,1) = ui->lineEdit12->text().toDouble(&ok);
    if(!ok)  mat(0,1)=-1;
     mat(0,2) = ui->lineEdit13->text().toDouble(&ok);
    if(!ok)  mat(0,2)=0;
     mat(1,0) = ui->lineEdit21->text().toDouble(&ok);
    if(!ok)  mat(1,0)=-1;
     mat(1,1) = ui->lineEdit22->text().toDouble(&ok);
    if(!ok)  mat(1,1)=5;
     mat(1,2) = ui->lineEdit23->text().toDouble(&ok);
    if(!ok)  mat(1,2)=-1;
     mat(2,0) = ui->lineEdit31->text().toDouble(&ok);
    if(!ok)  mat(0,1)=0;
     mat(2,1) = ui->lineEdit32->text().toDouble(&ok);
    if(!ok)  mat(2,1)=-1;
     mat(2,2)= ui->lineEdit33->text().toDouble(&ok);
    if(!ok)  mat(2,2)=0;
    bool spatial = ui->radiobtn_spatial->isChecked();

    emit confirmed(mat,spatial);
}
