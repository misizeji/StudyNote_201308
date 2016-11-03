#include <QtGui/QApplication>
#include "moviewin.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    moviewin w;
    w.show();

    return a.exec();
}
