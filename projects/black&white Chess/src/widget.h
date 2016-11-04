#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <phonon/VideoPlayer>
#include "myplayer.h"       //包含播放器头文件
#include "myboard.h"        //包含棋盘头文件
#include "udp_talk.h"       //添加聊天窗口

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    MyPlayer *Player;//视频播放器
    MyBoard *board; //棋盘
    udp_talk *mytalk;//聊天
    QTimer *timer;  //定时器
    Phonon::VideoPlayer *music_player;//背景音乐播放器
    QString pic_name;//背景图片名称
    int play_time;  //倒计时变量
    int music_player_flag;//背景音乐播放器标志位

private slots:
    void on_toolButton_talk_clicked();//聊天窗口
    void on_toolButton_tips_clicked();//提示
    void on_toolButton_clicked();//自动回放
    void on_toolButton_music_clicked();//背景音乐按钮
    void on_toolButton_Regret_clicked();//悔棋
    void on_toolButton_pvnet_clicked();//网络对战
    void on_toolButton_PVC_clicked();//人机对战
    void on_toolButton_PVP_clicked();//人人对战
    void on_WatchMovie_clicked();//观看电影
    void on_button_ChangeBG_clicked();//修改背景
    void update_score();    //更新分数
    void update_time();     //更新倒计时
    void lcd_time_show();   //显示倒计时
    void game_over_judge(); //游戏结束判断

protected:
    void paintEvent(QPaintEvent *);
    void set_time_count();      //设置定时器函数
};

#endif // WIDGET_H
