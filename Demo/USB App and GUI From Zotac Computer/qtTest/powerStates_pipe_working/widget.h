#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class QTimer;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

 //   void setName(const QString msg);
//   QString msg() const;

//private Q_SLOT:
public slots:

    void onReadyRead();

private:
    Ui::Widget *ui;
    QTimer *timer;
};

#endif // WIDGET_H
