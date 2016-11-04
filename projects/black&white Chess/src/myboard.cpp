#include "myboard.h"
#include "ui_myboard.h"

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QtNetwork>
#include <QPixmap>
#include <QPainter>
#include <QCursor>
#include <QTimer>
#include <QBitmap>
#include <QPaintEvent>

MyBoard::MyBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyBoard)
{
    ui->setupUi(this);
    //我先下棋
    this->Player_flag = 1;
    //设置棋子图片
    this->chess_pic_my = ":/new/pic/wc.png";
    this->chess_pic_other = ":/new/pic/bc.png";
    //初始化棋子数组
    //初始化后悔三维棋子数组
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            this->chess_put[i][j] = 0;
            this->chess_put_temp[i][j] = 0;
            this->chess_put_before[0][i][j] = this->chess_put[i][j];
        }
    }
    //中间四子分布
    chess_put[3][3] = 1;
    chess_put[3][4] = 2;
    chess_put[4][3] = 2;
    chess_put[4][4] = 1;
    //初始后悔三维棋子数组的四子
    chess_put_before[0][3][3] = 1;
    chess_put_before[0][3][4] = 2;
    chess_put_before[0][4][3] = 2;
    chess_put_before[0][4][4] = 1;
    //初始化鼠标形状
    this->setCursor(QCursor(QPixmap(chess_pic_my).scaled(80,80)));
    this->mouse_flag = 1;//鼠标响应事件标志位（响应）
    this->a = 0; this->b = 0;//初始化提示坐标
    this->put_times = 0;//初始化输入的下棋次数

    sender = new QUdpSocket(this);//客户端建立udp发送套接字
    receiver = new QUdpSocket(this);//服务器建立udp接收套接字
    receiver->bind(QHostAddress::Any,7000,QUdpSocket::ShareAddress);//绑定6666端口
    connect(receiver,SIGNAL(readyRead()),this,SLOT(processPendingDatagram()));//检测信号进入数据读取
}

MyBoard::~MyBoard()
{
    delete ui;
}

void MyBoard::game_init()//数据棋盘初始化
{
    //我先下棋
    this->Player_flag = 1;
    //设置棋子图片
    this->chess_pic_my = ":/new/pic/wc.png";
    this->chess_pic_other = ":/new/pic/bc.png";
    //初始化棋子数组
    //初始化后悔三维棋子数组
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            this->chess_put[i][j] = 0;
            this->chess_put_temp[i][j] = 0;
            this->chess_put_before[0][i][j] = this->chess_put[i][j];
        }
    }
    //中间四子分布
    chess_put[3][3] = 1;
    chess_put[3][4] = 2;
    chess_put[4][3] = 2;
    chess_put[4][4] = 1;

    //this->chess_put_temp[i][j] = 0;
    //初始后悔三维棋子数组的四子
    chess_put_before[0][3][3] = 1;
    chess_put_before[0][3][4] = 2;
    chess_put_before[0][4][3] = 2;
    chess_put_before[0][4][4] = 1;
    //初始化鼠标形状
    this->setCursor(QCursor(QPixmap(chess_pic_my).scaled(80,80)));
    this->mouse_flag = 1;//鼠标响应事件标志位（响应）
    this->a = 0; this->b = 0;//初始化提示坐标
    this->put_times = 0;//初始化输入的下棋次数
    this->update();//更新绘图
}

void MyBoard::paintEvent(QPaintEvent *)//画图事件
{
    //画棋盘
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/new/pic/lines.png"));

    //画棋子
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(this->chess_put_before[put_times][i][j] == 1)//我的棋子
            {
                QPainter q(this);
                q.drawPixmap((i*(this->width()/8)),(j*(this->height()/8)),\
                            (this->width()/8),(this->height()/8),QPixmap(chess_pic_my));
            }
            else if(this->chess_put_before[put_times][i][j] == 2)//他的棋子
            {
                QPainter q(this);
                q.drawPixmap((i*(this->width()/8)),(j*(this->height()/8)),\
                            (this->width()/8),(this->height()/8),QPixmap(chess_pic_other));
            }
            //只要恢复棋盘就将当前棋局也恢复（这样就不会在未恢复之前的棋局基础上下棋）
            this->chess_put[i][j] = this->chess_put_before[put_times][i][j];
            //将现有棋盘存入中间棋盘数组变量
            this->chess_put_temp[i][j] = this->chess_put_before[put_times][i][j];
        }
    }
}

void MyBoard::mousePressEvent(QMouseEvent *ev)//鼠标点击获得棋子坐标
{
    if(mouse_flag == 1) //响应鼠标事件
    {
        //获得棋子坐标
        int x = (ev->x()/(this->width()/8));//列
        int y = (ev->y()/(this->height()/8));//行
        if((chess_put[x][y] == 1)||(chess_put[x][y] == 2))//假设本身就有棋子
        {
            return;
        }
        else//坐标没棋子才是下棋的前提
        {
            if(this->play_mode == 1)    //人人对战模式
            {
                pvp_game_mode(x,y); //调用人人对战处理函数
            }
            else if(this->play_mode == 2)//人机对战模式
            {
                pvc_game_mode(x,y); //调用人机对战处理函数
            }
            else if(this->play_mode == 3)//网络对战模式
            {
                pvnet_game_mode(x,y);//调用网络对战函数
            }
            else
            {
                return;
            }
        }
        this->update();//更新绘图
    }
    else//不响应鼠标事件
    {
        return;
    }
}

int MyBoard::get_count(int role)//获得比分
{
    this->count = 0;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(chess_put_before[put_times][i][j] == role)
            {
                this->count++;
            }
        }
    }
    return this->count;
}

void MyBoard::set_put_before(int puttime)//悔棋画棋子 刷新计数
{
    put_times = puttime;//返回到上一层    (puttime = put_times -1)
    //恢复初局
    if(put_times <= 0)
    {
        put_times = 0;
        this->Player_flag = 1;
        this->setCursor(QCursor(QPixmap(chess_pic_my).scaled((this->width()/8),
                                                             (this->height()/8))));
    }
    else
    {
        player_flag_change();//更换选手标志
        if(play_mode == 2 && Player_flag == 2)//模式是人机，轮到机器人下棋
        {
            pvc_game_mode(3,4);//给个已知有子的坐标，使人重新下而不会下在(3,4)
        }
    }
    emit this->update_time_signal();//更新倒计时上限
    emit this->update_signal();//更新计分板信号
    //qDebug()<<"in set put before";
    this->update();//更新绘图
}

void MyBoard::player_flag_change()//更换选手标志函数
{
    //更换选手标志
    if(Player_flag == 1)
    {
        this->Player_flag = 2;
        //设置鼠标
        this->setCursor(QCursor(QPixmap(chess_pic_other).scaled((this->width()/8),
                                                             (this->height()/8))));
    }
    else if(Player_flag == 2)
    {
        this->Player_flag = 1;
        //设置鼠标
        this->setCursor(QCursor(QPixmap(chess_pic_my).scaled((this->width()/8),
                                                             (this->height()/8))));
    }
}

int MyBoard::my_judge(int x,int y,int gridNum, void *chess_array_addr, int currentRole)//算法函数
{
    //棋子走的8个方向
    int dir[8][2]={{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};
    if( x < 0 || x>= gridNum || y < 0 || y >= gridNum)	// 判断坐标是否合法
       return 0;
    //坐标传给中间变量
    int temp_x = x, temp_y = y;
    //吃子数初始化
    int eatNum = 0;
    typedef int (*p)[gridNum];//自定义类型
    p chessBoard = p(chess_array_addr);	//类型转换 并赋值给新变量 chessBoard
    //如果此方格内已有棋子，返回;
    if(chessBoard[temp_x][temp_y] != 0)
       return 0;
    for(int i = 0 ; i <8; i++)//循环判断8个方向
    {
        temp_x += dir[i][0]; temp_y += dir[i][1];//准备判断相邻棋子
        //如果没有出界，且相邻棋子是对方棋子，才有吃子的可能． 不是当前角色并且不为空（为对方棋子）
        if((temp_x < gridNum && temp_x >=0 && temp_y < gridNum && temp_y >= 0)\
            && (chessBoard[temp_x][temp_y] != currentRole)\
            && (chessBoard[temp_x][temp_y] != 0))
        {
            temp_x += dir[i][0]; temp_y += dir[i][1];//继续判断下一个

            //同样判断是否出界
            while(temp_x < gridNum && temp_x >=0 && temp_y < gridNum && temp_y >= 0)
            {
                //遇到空位跳出(没有找到自己的棋子)
                if(chessBoard[temp_x][temp_y] == 0)
                   break;
                //找到了自己的棋子，可以吃子
                if(chessBoard[temp_x][temp_y] == currentRole)
                {
                    chessBoard[x][y] = currentRole;//改变当前下子（x,y）坐标内的信息
                    temp_x -= dir[i][0]; temp_y -= dir[i][1];

                    while((temp_x != x )||(temp_y != y))//吃子的过程(找到自己最初的那个起点)
                    {
                        chessBoard[temp_x][temp_y] = currentRole;
                        temp_x -= dir[i][0]; temp_y -= dir[i][1];
                        eatNum ++;
                    }
                    break;
                }
                //如果仍然是对方棋子，继续沿此方向查找
                temp_x += dir[i][0]; temp_y += dir[i][1];
            }
        }
        temp_x = x; temp_y = y;	//一个方向判断结束，准备下一个方向
    }
    return eatNum;
}

bool MyBoard::iseated()//判断吃子函数
{
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            //如果中间变量数组坐标为空且能吃子
            if((chess_put_temp[i][j] == 0) && (my_judge(i,j,8,&chess_put_temp,Player_flag) > 0))
            {
                this->a = i;
                this->b = j;
                this->update();//刷新中间二维数组变量
                return true;
            }
        }
    }
    this->update();//刷新中间二维数组变量
    return false;
}

void MyBoard::pvp_game_mode(int x, int y)//人人对战处理函数
{
    if(iseated())//如果所有格子中有能吃的格子存在
    {
        if(this->my_judge(x,y,8,&chess_put,Player_flag) > 0)//能吃到棋子
        {
            this->put_times++;
            //棋子存入后悔三维棋子数组
            for(int i=0;i<8;i++)
            {
                for(int j=0;j<8;j++)
                {
                    this->chess_put_before[put_times][i][j] = chess_put[i][j];
                }
            }
            player_flag_change();//更换选手标志
            emit this->update_time_signal();//更新倒计时上限
            emit this->update_signal();//更新计分板信号
        }
    }
    else//如果所有格子中没有能吃的格子存在
    {
        player_flag_change();//更换选手标志
        if(iseated())   //判断对方能否吃子
        {
            return;
        }
        else//对方也没有下子机会
        {
            emit this->game_over();//游戏结束
        }
    }
    this->update();//更新绘图
}

void MyBoard::pvc_game_mode(int x, int y)//人机对战处理函数
{
    if(Player_flag == 1)
    {
        pvp_game_mode(x,y);
        this->mouse_flag = 0;   //屏蔽鼠标
        this->timer = new QTimer(this);
        this->t = 3;
        timer->start(1000);
        connect(this->timer,SIGNAL(timeout()),this,SLOT(timer_deal()));//进入倒计时函数进行机器下棋
    }
    else
    {
        this->mouse_flag = 0;   //屏蔽鼠标
        this->timer = new QTimer(this);
        this->t = 3;
        timer->start(1000);
        connect(this->timer,SIGNAL(timeout()),this,SLOT(timer_deal()));//进入倒计时函数进行机器下棋
        pvp_game_mode(x,y);
    }
}

void MyBoard::link_pvnet()//网络链接函数
{
    this->link_win = new mylink;
    this->link_win->setWindowTitle("Playing online");
    this->link_win->show();
    connect(this->link_win,SIGNAL(get_online()),this,SLOT(game_begin()));//对战游戏开始
}

void MyBoard::game_begin()//网络告知对方自己的标志位
{
    msg_send(9,0,Player_flag);//将我的标志位发给对方
}

void MyBoard::pvnet_game_mode(int x, int y)//网络对战函数
{
    if(iseated())//如果所有格子中有能吃的格子存在
    {
        if(this->my_judge(x,y,8,&chess_put,Player_flag) > 0)//能吃到棋子
        {
            msg_send(x,y,Player_flag);
            this->put_times++;
            //棋子存入后悔三维棋子数组
            for(int i=0;i<8;i++)
            {
                for(int j=0;j<8;j++)
                {
                    this->chess_put_before[put_times][i][j] = chess_put[i][j];
                }
            }
            //player_flag_change();//更换选手标志
            mouse_flag = 0;//屏蔽鼠标 等待对方下棋
            emit this->update_time_signal();//更新倒计时上限
            emit this->update_signal();//更新计分板信号
        }
    }
    else//如果所有格子中没有能吃的格子存在
    {
        player_flag_change();//更换选手标志
        if(iseated())   //判断对方能否吃子
        {
            player_flag_change();//更换选手标志
            msg_send(10,0,Player_flag);
            return;
        }
        else//对方也没有下子机会
        {
            emit this->game_over();//游戏结束
        }
    }
    this->update();//更新绘图

}

void MyBoard::timer_deal()//倒计时函数 机器下棋
{
    t--;
    if(t == 0)
    {
        timer->stop();
        //机器下
        if(iseated()) //得到机器下子坐标(a,b)
        {
            this->my_judge(this->a,this->b,8,&chess_put,Player_flag);//直接改变棋局
            this->put_times++;
            //棋子存入后悔三维棋子数组
            for(int i=0;i<8;i++)
            {
                for(int j=0;j<8;j++)
                {
                    this->chess_put_before[put_times][i][j] = chess_put[i][j];
                }
            }
            player_flag_change();//更换选手标志
            emit this->update_time_signal();//更新倒计时上限
            emit this->update_signal();//更新计分板信号
        }
        else//如果所有格子中没有能吃的格子存在
        {
            player_flag_change();//更换选手标志
            if(iseated())   //判断对方能否吃子
            {
                return;
            }
            else//对方也没有下子机会
            {
                emit this->game_over();//游戏结束
            }
        }
        this->update();//更新绘图
        this->mouse_flag = 1;
    }
}

void MyBoard::refresh()//回放函数
{
    if(put_times <= 0)
    {
        return;
    }
    else
    {
        this->put_times_before = put_times;//记录当前下棋次数
        this->times_temp = 0;   //设置中间循环变量
        this->refresh_timer = new QTimer(this);
        this->refresh_timer->start(1000);
        //调用函数进入递归
        connect(this->refresh_timer,SIGNAL(timeout()),this,SLOT(refresh_deal()));//进入倒计时函数进行回放下棋
    }
}

void MyBoard::refresh_deal()//递归调用函数回放棋盘
{
    this->refresh_timer->stop();
    this->mouse_flag = 0;   //屏蔽鼠标
    this->put_times = this->times_temp;
    emit this->update_time_signal();//更新倒计时上限
    emit this->update_signal();//更新计分板信号
    this->update();
    this->times_temp++;
    if(times_temp == this->put_times_before)
    {
        this->mouse_flag = 1;   //响应鼠标
        this->put_times = put_times_before;
        this->update();
        return;
    }
    this->refresh_timer = new QTimer(this);
    this->refresh_timer->start(1000);
    //函数递归使用
    connect(this->refresh_timer,SIGNAL(timeout()),this,SLOT(refresh_deal()));//进入倒计时函数进行回放下棋
}

QString MyBoard::put_tips()//提示下子函数
{
    iseated();
    QString tip = QString("(%1,%2)").arg(this->a).arg(this->b);
    return tip;
}

void MyBoard::msg_send(int x,int y,int flag)//发送函数
{
    QString send_msg = QString("%1:%2:%3").arg(x).arg(y).arg(flag);
    QByteArray datagram = send_msg.toAscii();
        sender->writeDatagram(datagram.data(),datagram.size(),
                              QHostAddress(this->link_win->get_ip()),this->link_win->get_port().toInt());
}

void MyBoard::processPendingDatagram() //处理等待的数据包
{
    QByteArray datagram; //用于存放接收的数据报
    while(receiver->hasPendingDatagrams())  //拥有等待的数据报
    {
        datagram.resize(receiver->pendingDatagramSize());
        //让datagram的大小为等待处理的数据报的大小，这样才能接收到完整的数据
        receiver->readDatagram(datagram.data(),datagram.size());
        //接收数据报，将其存放到datagram中
    }
    //将数据报内容显示出来
    //qDebug()<<datagram;
    QString str = datagram;
    QStringList recv_list = str.split(":");
    qDebug()<<recv_list[0]<<" "<<recv_list[1]<<" "<<recv_list[2]<<" ";
    QString str_x = recv_list[0];
    QString str_y = recv_list[1];
    QString str_flag = recv_list[2];

    if(str_x.toInt() == 9)//游戏开始（通知彼此下棋标志位）
    {
        if(str_flag.toInt() == 1)
        {
            this->Player_flag = 2;
            this->my_flag = 2;
            this->setCursor(QCursor(QPixmap(chess_pic_other).scaled((this->width()/8),
                                                                 (this->height()/8))));
        }
        else
        {
            this->Player_flag = 1;
            this->my_flag = 1;
            this->setCursor(QCursor(QPixmap(chess_pic_my).scaled((this->width()/8),
                                                                 (this->height()/8))));
        }
    }
    else if(str_x.toInt() == 10)//（对方不能下棋）
    {
        mouse_flag = 1; //响应鼠标己方下棋
    }
    else//告知下棋
    {
        my_judge(str_x.toInt(),str_y.toInt(),8,&chess_put,str_flag.toInt());
        this->put_times++;
        //棋子存入后悔三维棋子数组
        for(int i=0;i<8;i++)
        {
            for(int j=0;j<8;j++)
            {
                this->chess_put_before[put_times][i][j] = chess_put[i][j];
            }
        }
        //player_flag_change();//更换选手标志
        mouse_flag = 1; //响应鼠标己方下棋
        emit this->update_time_signal();//更新倒计时上限
        emit this->update_signal();//更新计分板信号
    }
    this->update();
}
