#include "widget.h"
#include "ui_widget.h"

#include <QtCore>
#include <qtextstream.h>
#include <qstring.h>

#include <stdio.h>
#include <stdlib.h>

QTextStream qtin(stdin);
//QString line = qtin.readLine(); // This is how you read the entire line

QString word;

int cnt;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);


    // Sleep
    ui->label->hide();
    ui->label_2->show();
    ui->label_3->hide();

  timer = new QTimer(this);
  timer->setInterval(4000);

  connect(timer, SIGNAL(timeout()), this, SLOT(onReadyRead()));
  timer->start(4000);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::onReadyRead()
{
   qtin >> word;

/*
cnt++;

if ( cnt % 2 == 0)
{
    // Active On
    ui->label->show();
    ui->label_2->hide();
    ui->label_3->hide();


}
else
{
    // Sleep
    ui->label->hide();
    ui->label_2->show();
    ui->label_3->hide();

}
*/

    if ( word.at(0) == '0')
    {
        // Active On
        ui->label->show();
        ui->label_2->hide();
        ui->label_3->hide();
    }
    else if  ( word.at(0) == '1')
    {
        // Sleep
        ui->label->hide();
        ui->label_2->show();
        ui->label_3->hide();
    }
    else if  ( word.at(0) == '2')
    {
        // Deep Sleep
        ui->label->hide();
        ui->label_2->hide();
        ui->label_3->show();


    }
    else
    {
        // error
        ui->label->hide();
        ui->label_2->hide();
        ui->label_3->hide();

    }


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

