#ifndef MYBOARD_H
#define MYBOARD_H

#include <QWidget>
#include <QtNetwork>
#include "mylink.h"//添加链接头文件

namespace Ui {
    class MyBoard;
}

class MyBoard : public QWidget
{
    Q_OBJECT

public:
    explicit MyBoard(QWidget *parent = 0);
    ~MyBoard();
    int mouse_flag;    //鼠标屏蔽响应标志位
    int get_count(int);//获得玩家棋子数
    int put_times;   //当前下棋的次数
    int Player_flag; //选手标志(同时也代表数组要存储的数字)
    int my_flag;     //网络对战记录自己的标志位
    int play_mode;   //游戏模式(0代表未开始，1代表人人对战，2代表人机对战，3代表网络对战)
    void pvc_game_mode(int x,int y);    //人机对战处理函数
    void pvp_game_mode(int x,int y);    //人人对战模式处理函数
    void pvnet_game_mode(int x,int y);  //网络对战函数
    void link_pvnet();                  //网络链接函数
    void game_init();   //游戏数据初始化
    void refresh();//自动回放函数
    QString put_tips();//提示下子（一个位置）


public slots:
    void set_put_before(int);  //恢复棋盘
    void player_flag_change();//更换选手标志
    void timer_deal();         //机器下棋处理函数
    void refresh_deal();       //回放处理函数
    void msg_send(int x,int y,int flag);//发送棋盘信息函数
    void game_begin();         //网络对战开始
    void processPendingDatagram();//服务器接收数据

private:
    Ui::MyBoard *ui;
    QUdpSocket *sender;//客户端
    QUdpSocket *receiver;//服务器
    QTimer *timer;          //机器下棋倒计时
    QTimer *refresh_timer;  //自动回放倒计时
    mylink *link_win;   //网络对战信息窗口
    int count;              //玩家棋子数
    int chess_put[8][8];    //棋子数组
    int chess_put_temp[8][8];   //吃子函数判断中间变量数组
    int chess_put_before[61][8][8];//后悔棋子三维数组
    QString chess_pic_my;      //自己棋子图片
    QString chess_pic_other;   //别人棋子图片
    int a,b;                   //机器能下的坐标
    int t;                  //定时秒数
    int put_times_before;   //记录下棋次数
    int times_temp;         //回放中间循环变量


signals:
    void update_signal();   //刷新记分板信号
    void update_time_signal();  //刷新时间信号
    void game_over();           //游戏结束信号

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    bool iseated();    //判断吃子函数
    int my_judge(int x,int y,int gridNum, void *chess_array_addr, int currentRole); //算法

};

#endif // MYBOARD_H
