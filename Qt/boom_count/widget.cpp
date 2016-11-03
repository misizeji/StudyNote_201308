#include "widget.h"
#include "ui_widget.h"
#include <QMovie>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->label->hide();
    this->timer = new QTimer(this);//设置定时器
    connect(timer,SIGNAL(timeout()),this,SLOT(myslot()));//链接定时器信号
    timer->start(1000);
    t = 10;
    ui->lcdNumber->display(t);//lcd显示数字
}

Widget::~Widget()
{
    delete ui;
}

void Widget::myslot()
{
    t--;
    if(t == 0)
    {
        ui->lcdNumber->close();	//计时结束关闭lcd窗口
        QMovie *movie = new QMovie(this);//设置movie变量
        movie->setFileName(":/new/pic/boom.gif");//设置gif动画图片
        ui->label->setMovie(movie);//让label显示图片
        movie->start();//动画显示
        ui->label->setScaledContents(true);//动画自动填充
        //ui->label->setWindowFlags(Qt::Window);
        //ui->label->showFullScreen();
        this->showFullScreen(); //窗口全屏
        //this->setGeometry(0,0,1440,900);
        ui->label->setGeometry(0,0,1440,900);//图片窗口设置成桌面大小
        ui->label->show();//label显示
    }
    ui->lcdNumber->display(t);//继续倒计时
}
