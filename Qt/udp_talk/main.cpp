#include <QtGui/QApplication>
#include "udp_talk.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    udp_talk w;
    w.show();

    return a.exec();
}
