#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

//    w.setName("USB Msg");
    w.show();



    return a.exec();
}
