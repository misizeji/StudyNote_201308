#include "widget.h"
#include "ui_widget.h"

#include <QPixmap>
#include <QLabel>
#include <QMouseEvent>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->label->resize(20,20);	//标签设置大小
    ui->label->setPixmap(QPixmap("../image/face-1.png").scaled(20,20));//标签贴图
    ui->label->setScaledContents(true);//自动适应大小
    this->x = ui->label->x();
    this->y = ui->label->y();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::mousePressEvent(QMouseEvent *ev)		//鼠标左右键控制图片移动
{
    int button = ev->button();		//获取按键值
    if(button == Qt::RightButton)//左键
    {
        this->x = x + 20;
        if((x+ui->label->width()) >this->width())
        {
            x = 0;
        }
        ui->label->move(x,y);
    }
    if(button == Qt::LeftButton)//右键
    {
        this->x = x - 20;
        if(x < 0)
        {
            x = this->width()-ui->label->width();
        }
        ui->label->move(x,y);
    }

}

void Widget::wheelEvent(QWheelEvent *ev)//鼠标滚轮事件
{
    int w = ui->label->width();
    int h = ui->label->height();
    if(ev->orientation() == Qt::Vertical)	//鼠标滚轮有效
    {
        if(ev->delta() > 0)	//滚轮向上
        {
            ui->label->resize(++w,++h);
        }
        else//滚轮向下
        {
            ui->label->resize(--w,--h);
        }
    }
}

void Widget::keyPressEvent(QKeyEvent *ev)//键盘按键事件
{
    int step = 20;
    int button = ev->key();
    if(ev->modifiers() == Qt::CTRL)//按键同时又组合按键按下
    {
        step = 10;		//改变移动步伐大小
    }

    if (button == Qt::Key_Up)//键盘方向向上键
    {
        y = y - step;
        if(y < 0)
        {
            y = 0;
        }
        ui->label->move(x,y);//标签移动
    }
    else if(button == Qt::Key_Down)//键盘方向键向下键
    {
        y = y + step;
        if(y+ui->label->height() > this->height())
        {
            y = this->height() - ui->label->height();
        }
        ui->label->move(x,y);
    }
    else if(button == Qt::Key_Left)//方向左
    {
        x = x - step;
        if(x < 0)
        {
            x = 0;
        }
        ui->label->move(x,y);
    }
    else if(button == Qt::Key_Right)//方向右
    {
        x = x + step;
        if(x+ui->label->width() > this->width())
        {
            x = this->width() - ui->label->width();
        }
        ui->label->move(x,y);
    }
}

