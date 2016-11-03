#ifndef WIDGET_H
#define WIDGET_H


#include <QWidget>
#include <QTimer>
#include <QPoint>
#include <QMouseEvent>
#include <QDebug>
#include <QPen>
#include <QPainter>
#include <QMessageBox>
#include "chessboard.h"

namespace Ui {
    class Widget;
}

// 主窗口
class Widget : public QWidget {
    Q_OBJECT
public:
    Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    // 槽函数，接收棋盘窗口的信号
    // 参数为棋盘下棋的坐标
    void doPressPoint(QPoint);

protected:
    // 绘图事件，主要是画当然窗口的背景
    void paintEvent(QPaintEvent *);

private:

    Ui::Widget *ui;
    ChessBoard *board;  // 棋盘类的对象
    bool isStart;
    bool PVPFlag;       // 人人对战的标志位
    ChessBoard::GridState currentRole;  // 枚举变量，棋盘状态的标志位
    int chess[64][8][8];    // 三维数组，方便标志棋盘状态
    int step;               // 标志当前下子为第几步
    int gridNum;
    int times;
    int failFlag;
    QTimer timer;           // 倒计时
    QTimer machineTime;     // 机器的时间，延时作用
    void showScore();       // 显示分数(黑白子各自的数目)
    void changeRole();      // 改变角色

    // 吃子的规则
    // 吃子规则的参数：棋盘数组坐标位置(x y)、横着或竖着的格数(gridNum)、
    // 棋子状态(chess)、棋子的优先权(注意其变量的写法)、eatChess默认为true
    int judgeRule(int, int, int , void *, ChessBoard::GridState,bool eatChess=true);
    void init();    // 初始化

private slots:
    void on_toolButtonOver_clicked();   // 游戏结束按钮
    void on_toolButtonJump_clicked();   // 跳过按钮
    void on_toolButtonGoback_clicked(); // 悔棋按钮
    void on_toolButtonMode_clicked();   // 模式(人人或人人)选择按钮
    void on_toolButtonStart_clicked();  // 开机按钮
    void timerSlot();   // 倒计时的槽函数
    void PVEmethod();   // 机器的槽函数
};

#endif // WIDGET_H
