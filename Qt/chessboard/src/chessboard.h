#ifndef CHESSBOARD_H
#define CHESSBOARD_H

// 包含所需要的头文件
#include <QtGui/QWidget>
#include <QPen>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

// 棋盘类
class ChessBoard : public QWidget
{
    Q_OBJECT

public:
    // 构造函数
    // fileName为棋盘背景图的路径
    // 棋盘网格数8*8，所以默认参数为8
    ChessBoard(QWidget *parent = 0 ,
               QString fileName = "",
               int num = 8);
    ~ChessBoard();  // 析构函数

    // 枚举变量
    // Empty标志为空，Black标志为黑子，White标志为白子
    enum GridState{Empty,Black, White };

    int getGrid();                  // 获得棋盘一个小方格的宽度(或高度)
    void setPen(const QPen & pen);  // 设置画笔

    // 设置棋盘状态，manInfo为数组的首地址，(px,py)为当前下子的坐标
    void setChessManInfo(void *manInfo,int px = -1,int py = -1);

protected:
    // 绘图事件，主要功能是画棋盘，棋子
    void paintEvent(QPaintEvent *);

    // resize事件，当改变窗口时自动调用
    // 此函数功能是获得窗口最新的宽度和高度，通过这来确定棋盘的起点坐标
    void resizeEvent(QResizeEvent *);

    // 鼠标点击事件，主要是确定棋盘方格的位置
    void mousePressEvent(QMouseEvent *);

private:
    int grid;           // 棋盘一个小方格的宽度(或高度),这里宽度和高度相等
    int startx,starty;  // 棋盘的起点坐标(startx,starty)
    int gridNum;        // 横着或竖着的格数，这里棋盘为8*8，所以为8

    // (px,py)为当前下子的坐标
    int px;
    int py;

    // 棋盘状态指针，方便接收从主窗口传过来的棋盘状态
    void *chessManInfo;
    QPen myPen;                 // 画笔对象
    QString backImageFileName;  // 用来保存棋盘图片的路径

signals:
    // 鼠标点击后，给主窗口发射的信号
    // QPoint参数为当前下子的坐标
    void pressPoint(QPoint);
};

#endif // CHESSBOARD_H
