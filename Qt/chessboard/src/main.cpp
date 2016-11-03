#include <QtGui/QApplication>
#include "widget.h"
#include "chessboard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

   //w.showFullScreen();//arm板时全屏显示
    w.show();           //PC机运行，带窗口标题和边框
    return a.exec();
}
