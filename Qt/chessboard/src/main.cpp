#include <QtGui/QApplication>
#include "widget.h"
#include "chessboard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

   //w.showFullScreen();//arm��ʱȫ����ʾ
    w.show();           //PC�����У������ڱ���ͱ߿�
    return a.exec();
}
