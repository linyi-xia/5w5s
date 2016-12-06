#include "widget.h"
#include "ui_widget.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char buf;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);


    // Active On
    ui->label->show();
    ui->label_2->hide();
    ui->label_3->hide();


}

Widget::~Widget()
{
    delete ui;
}
/*
void Widget::setName(const QString msg)
{

    ui->lineEdit->setText(msg);
}


QString Widget::msg() const
{
    return ui->lineEdit->text();
}
*/
