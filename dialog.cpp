#include"dialog.h"
#include "ui_dialog.h"
#include<iostream>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_buttonBox_accepted(){
    std::cout<<"button box accepted"<<std::endl;
    bool ok;
    double c = ui->lineEdit->text().toDouble(&ok);
    if(ok)
        emit confirmed(c); //r为0～1
}
