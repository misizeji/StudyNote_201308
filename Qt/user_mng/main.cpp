#include <QtGui/QApplication>
#include "usermngwin.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UserMngWin w;
    w.show();

    return a.exec();
}
