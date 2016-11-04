#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QString>
#include <QDateTime>
#include <QPixmap>
#include <QPalette>
#include <QStringList>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QStringList>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //初始化计分板
    ui->MyScore->setPixmap(QPixmap(":/new/num/Numbers/0.png"));
    ui->MyScore->setScaledContents(true);
    ui->OtherScore->setPixmap(QPixmap(":/new/num/Numbers/0.png"));
    ui->OtherScore->setScaledContents(true);

    //添加音乐
    this->music_player = new Phonon::VideoPlayer(Phonon::MusicCategory);
    music_player->play(Phonon::MediaSource("../blackwhite_chess/media_source/dongtian.wav"));
    this->music_player_flag = 1;

    //添加棋盘
    this->board = new MyBoard(this);
    ui->gridLayout->addWidget(board,0,1);
    this->board->play_mode = 0;

    //画背景图片(初始背景图片的名称)
    pic_name = ":/new/BG_picture/3d_boy/6.jpg";

    //设置定时器
    this->timer = new QTimer(this);

    connect(this->board,SIGNAL(update_signal()),this,SLOT(update_score()));//刷新计分板
    connect(this->board,SIGNAL(update_time_signal()),this,SLOT(update_time()));//刷新倒计时上限,开始计时
    connect(timer,SIGNAL(timeout()),this,SLOT(lcd_time_show()));//lcd倒计时板显示
    connect(this->board,SIGNAL(game_over()),this,SLOT(game_over_judge()));  //判断输赢

}

Widget::~Widget()
{
    delete ui;
    delete board;
    delete timer;
    delete music_player;
}

void Widget::paintEvent(QPaintEvent *)//画图事件画背景图片
{
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap(pic_name));
}

void Widget::on_button_ChangeBG_clicked()//改变背景
{
    static int i_before = 0;   //设置图片切换变量存储i
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));  //设置时间种子
    int t = qrand();   //获取当前时间种子
    int i = (t%17);     //获取0-16之间随机数
    if(i == i_before)
    {
       i = i + 1;      //如果i=4的话，i+1就是5，等于恰好随机到第6张图片
    }
    QStringList pic_list;   //建立字符串图片数组
    pic_list<<":/new/BG_picture/3d_boy/1.jpg"<<":/new/BG_picture/3d_boy/2.jpg"<<":/new/BG_picture/3d_boy/3.jpg"
            <<":/new/BG_picture/3d_boy/4.jpg"<<":/new/BG_picture/3d_boy/5.jpg"<<":/new/BG_picture/3d_boy/6.jpg"
            <<":/new/BG_picture/3d_boy/7.jpg"<<":/new/BG_picture/3d_boy/8.jpg"<<":/new/BG_picture/3d_boy/9.jpg"
            <<":/new/BG_picture/3d_boy/10.jpg"<<":/new/BG_picture/3d_boy/11.jpg"<<":/new/BG_picture/3d_boy/12.jpg"
            <<":/new/BG_picture/3d_boy/13.jpg"<<":/new/BG_picture/3d_boy/14.jpg"<<":/new/BG_picture/3d_boy/15.jpg"
            <<":/new/BG_picture/3d_boy/16.jpg"<<":/new/BG_picture/3d_boy/17.jpg"<<":/new/BG_picture/3d_boy/0.jpg";
    this->pic_name = pic_list[i];   //修改画图图片的名称
    i_before = i;
    this->update();    //刷新画图
}

void Widget::on_toolButton_music_clicked()//音乐播放
{
    if(this->music_player_flag == 1)
    {
        this->music_player->pause();
        this->music_player_flag = 0;
    }
    else
    {
        music_player->play(Phonon::MediaSource("../blackwhite_chess/media_source/dongtian.wav"));
        this->music_player_flag = 1;
    }
}

void Widget::on_WatchMovie_clicked()//观看视频
{
    //添加播放器
    this->Player = new MyPlayer;
    this->Player->move(385,70);
    this->Player->show();
}

void Widget::on_toolButton_PVP_clicked()//人人对战
{
    this->board->game_init();//初始化数据
    this->board->play_mode = 1;//设置游戏模式
    set_time_count();   //设置定时器
    ui->toolButton_PVP->setEnabled(false);
    ui->toolButton_PVC->setEnabled(false);
    ui->toolButton_pvnet->setEnabled(false);
}

void Widget::on_toolButton_PVC_clicked()//人机对战
{
    this->board->game_init();//初始化数据
    this->board->play_mode = 2;//设置游戏模式
    set_time_count();   //设置定时器
    ui->toolButton_PVP->setEnabled(false);
    ui->toolButton_PVC->setEnabled(false);
    ui->toolButton_pvnet->setEnabled(false);
}

void Widget::on_toolButton_pvnet_clicked()//网络对战函数
{
    this->board->game_init();//初始化数据
    this->board->play_mode = 3;//设置游戏模式
    this->board->link_pvnet();//打开链接窗口
    ui->toolButton_PVP->setEnabled(false);
    ui->toolButton_PVC->setEnabled(false);
    ui->toolButton_pvnet->setEnabled(false);
    ui->toolButton_Regret->setEnabled(false);
}

void Widget::on_toolButton_Regret_clicked()//悔棋
{
    if(this->board->play_mode == 0)//未开始或游戏结束
    {
        return;
    }
    else
    {
        this->board->set_put_before(this->board->put_times-1);
    }
}

void Widget::update_score()//刷新计分板函数
{
    QString mycount_str = QString(":/new/num/Numbers/%1.png").arg(board->get_count(1));
    QString othercount_str = QString(":/new/num/Numbers/%1.png").arg(board->get_count(2));
    ui->MyScore->setPixmap(QPixmap(mycount_str));
    ui->OtherScore->setPixmap(QPixmap(othercount_str));

    //游戏结束棋盘下满
    if((board->get_count(1) + board->get_count(2)) == 64)
    {
        game_over_judge();//判断输赢
    }
}

void Widget::set_time_count()//设置时间
{
    this->play_time = 30;
    ui->lcdNumber_time->display(play_time);
    this->timer->start(1000);
}

void Widget::update_time()//刷新倒计时上限,开始计时
{
    //qDebug()<<"in the update_time";
    this->timer->stop();//定时器重置
    set_time_count();   //设置定时器
}

void Widget::lcd_time_show()//lcd计时显示
{
    //qDebug()<<"in the lcd_time_show";
    this->play_time--;
    if(this->play_time == 0)
    {
        if(this->board->play_mode == 3 && this->board->Player_flag == this->board->my_flag)
        {
            this->board->msg_send(10,0,this->board->my_flag);//解除对方响应
            this->board->mouse_flag = 0;//屏蔽自己的鼠标
            this->play_time = 30;
            ui->lcdNumber_time->display(this->play_time);
            return;
        }

        this->board->player_flag_change();//更换选手
        this->play_time = 30;
        //人不下(时间过去了)，让机器人先下
        if(this->board->play_mode == 2 && this->board->Player_flag == 2)//模式是人机，轮到机器人下棋
        {
            this->board->pvc_game_mode(3,4);//给个已知有子的坐标，使人重新下而不会下在(3,4)
        }
    }
    ui->lcdNumber_time->display(this->play_time);
}

void Widget::game_over_judge()//游戏结束输赢判断
{
    this->board->play_mode = 0;//棋盘结束鼠标响应
    this->timer->stop();//定时器结束
    ui->lcdNumber_time->display(0);//倒计时显示0

    if(board->get_count(1) == board->get_count(2))
    {
        QMessageBox::information(this,"info","Game Over\nboth win!!!",QMessageBox::Ok);
    }
    else if(board->get_count(1) > board->get_count(2))
    {
        QMessageBox::information(this,"info","Game Over\nwhite win!!!",QMessageBox::Ok);
    }
    else if(board->get_count(1) < board->get_count(2))
    {
        QMessageBox::information(this,"info","Game Over\nblack win!!!",QMessageBox::Ok);
    }
    ui->toolButton_PVP->setEnabled(true);
    ui->toolButton_PVC->setEnabled(true);
    ui->toolButton_pvnet->setEnabled(true);
    ui->toolButton_Regret->setEnabled(true);
}

void Widget::on_toolButton_clicked()//自动回放函数
{
    this->board->refresh();
}

void Widget::on_toolButton_tips_clicked()//下子位置提醒函数
{
    if(this->board->play_mode == 0)
        return;
    QString tips = this->board->put_tips();
    ui->label_tipshow->setText(tips);
}

void Widget::on_toolButton_talk_clicked()//网络聊天
{
    this->mytalk = new udp_talk;
    this->mytalk->show();
}
