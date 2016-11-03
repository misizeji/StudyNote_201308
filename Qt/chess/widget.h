#ifndef WIDGET_H
#define WIDGET_H

#define GO   0
#define BACK 1
#define ON   0
#define OFF  1

#include <QWidget>
#include <QStringList>
#include <QTimer>
#include <QSound>
#include <QMouseEvent>
#include "chessbox.h"
#include "startmovie.h"
#include "startwindow.h"

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void setBackGroundName();
    void initChess();
    void copyChess(int [SIZE][SIZE], int [SIZE][SIZE]);
    int getChessNum(int, int);
    void initConnect();
    void changeRole(int );

private:
    Ui::Widget *ui;
    chessBox *box;
    StartMovie *sMovie;
    StartWindow *sWindow;
    QStringList background; //保存背景图路径名称
    QTimer *timer;
    QTimer *delay;
    QTimer *songTime;
    QTimer *wTime;
    QSound *backsong;
    int chess[SIZE*SIZE*SIZE][SIZE][SIZE];
    int count;  //背景显示序号
    int current;    //当前棋子颜色
    int currentPage;    //当前棋盘页码
    int flags[60];  //记录每次棋手
    int remind[60]; //记录每次提示
    int whiteNum;   //记录白子个数
    int blackNum;   //记录黑子个数
    int t;  //倒计时
    int playMode;     //电脑难度
    int lock;  //下棋锁
    int wFlag;
    int sizeFlag;
    int sizeChange;
    int maxflag;
    int moveflag;
    int moveW;
    int moveH;
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
signals:
    void pvc();
    void isEnd();
    void backToStart();
private slots:
    void on_quitToolButton_clicked();
    void on_maxToolButton_clicked();
    void on_minToolButton_clicked();
    void on_endButton_clicked();
    void on_giveupButton_clicked();
    void on_checkToolButton_clicked();
    void on_startButton_clicked();
    void doDrawPos(int, int);
    void doTimeout();
    void doSongTimeOut();
    void doBegin(int);
    void doPvc();
    void doDelay();
    void doIsEnd();
    void doWTimeout();
    void on_passButton_clicked();
    void on_gobackButton_clicked();
    void on_switchButton_clicked();
};

#endif // WIDGET_H
