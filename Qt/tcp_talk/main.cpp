#include <QtGui/QApplication>
#include "tcp_talk.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    tcp_talk w;
    w.show();

    return a.exec();
}
