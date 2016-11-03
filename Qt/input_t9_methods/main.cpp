#include <QtGui/QApplication>
#include "widget.h"
#include "t9input/t9widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    //T9Widget x;
    //x.show();

    return a.exec();
}
