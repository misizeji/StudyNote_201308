#include <QtGui/QApplication>
#include "widget.h"

#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("gb18030"));

    QApplication a(argc, argv);
    Widget w;
    w.show();


    return a.exec();
}
