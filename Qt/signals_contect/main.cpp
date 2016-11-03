#include <QtGui/QApplication>
#include "widget.h"
#include "myform.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    MyForm m;
    //m.show();

    return a.exec();
}
