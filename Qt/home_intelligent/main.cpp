#include <QtGui/QApplication>
#include "connection.h"
#include "mainwidget.h"
#include "options.h"

int show_pic_delay = 5;
int show_menu_delay = 30;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (!createConnection())
    {
        return -1;
    }

    mainWidget w;
    w.show();
    return a.exec();
}
