#include <QtGui/QApplication>
#include <QTextCodec> //�ı�����
#include <QWidget>
#include "widget.h"
#include "startwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForCStrings(
            QTextCodec::codecForName("gb18030"));
    Widget w;
   // w.show();


    return a.exec();
}
