#include "widget.h"
#include "ui_widget.h"

#include <QtCore>
#include <qtextstream.h>
#include <qstring.h>

#include <stdio.h>
#include <stdlib.h>

QTextStream qtin(stdin);
QString word;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);


    // Sleep
    ui->label->hide();
    ui->label_2->hide();
    ui->label_3->show();

  timer = new QTimer(this);
  timer->setInterval(4000);

  connect(timer, SIGNAL(timeout()), this, SLOT(onReadyRead()));
  timer->start(100);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::onReadyRead()
{
   qtin >> word;


    if ( word.at(0) == '0')
    {
        // Deep Sleep
        ui->label->hide();
        ui->label_2->hide();
        ui->label_3->show();
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
        // Active On
        ui->label->show();
        ui->label_2->hide();
        ui->label_3->hide();

    }
    else
    {
        // error
        ui->label->hide();
        ui->label_2->hide();
        ui->label_3->hide();

    }


}



