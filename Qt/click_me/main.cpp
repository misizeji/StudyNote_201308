#include <QtGui/QApplication>
#include "mywidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    myWidget w;
    w.show();
    return a.exec();
}
