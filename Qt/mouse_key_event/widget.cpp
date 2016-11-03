#include "widget.h"
#include "ui_widget.h"
#include <QMouseEvent>
#include <QDebug>
#include <QString>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QResizeEvent>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setMouseTracking(true);//设置鼠标追踪
}

Widget::~Widget()
{
    delete ui;
}

void Widget::mousePressEvent(QMouseEvent *ev)//鼠标按下事件
{
    int x = ev->x();//鼠标点击的x坐标
    int y = ev->y();//鼠标点击的y坐标
    int button = ev->button();//鼠标的按键获取
    
	QString str;
    if(button == Qt::LeftButton)//鼠标左键
    {
        str = QString("LeftButton Pressed \n(%1,%2)").arg(x).arg(y);
    }
    else if(button == Qt::RightButton)//鼠标右键
    {
        str = QString("RinghtButton Pressed \n(%1,%2)").arg(x).arg(y);
    }
    else if(button == Qt::MiddleButton)//鼠标中键
    {
        str = QString("MidButtton Pressed \n(%1,%2)").arg(x).arg(y);
    }
    ui->label->setText(str);
    qDebug()<<"str is "<<str;
    qDebug()<<"x = "<<x
            <<" y = "<<y;
    qDebug()<<ev->pos();//显示鼠标的pos
}

void Widget::mouseReleaseEvent(QMouseEvent *ev)//鼠标提起事件
{
    int x = ev->x();
    int y = ev->y();
    int button = ev->button();//获取鼠标按键
    QString str;
    if(button == Qt::LeftButton)//鼠标左键提起
    {
        str = QString("LeftButton Released \n(%1,%2)").arg(x).arg(y);
    }
    else if (button == Qt::MidButton)//鼠标中间提起
    {
        str = QString("MidButton Released \n(%1,%2)").arg(x).arg(y);
    }
    else if(button == Qt::RightButton)//鼠标右键提起
    {
        str = QString("RightButton Released \n(%1,%2)").arg(x).arg(y);
    }
    ui->label->setText(str);
}

void Widget::mouseMoveEvent(QMouseEvent *ev)//鼠标移动事件
{
    int x = ev->x();
    int y = ev->y();
    QString str;
    str = QString("The Pos is (%1,%2)").arg(x).arg(y);
    ui->label->setText(str);
}

void Widget::wheelEvent(QWheelEvent *ev)//鼠标滚轮事件
{
    if(ev->orientation() == Qt::Vertical)//
    {
        if(ev->delta() > 0)
        {
            ui->label->setText("go up");
        }
        else
        {
            ui->label->setText("go down");
        }
    }
    else
    {
        return;
    }
}

void Widget::keyPressEvent(QKeyEvent *ev)//键盘按键判断检测
{

    QString str;
    int m = ev->modifiers();//修饰键按下判断
    switch(m)
    {
    case Qt::SHIFT:
       str = "SHIFT + ";
       break;
    case Qt::ALT:
       str = "ALT + ";
       break;
    case Qt::CTRL:
       str = "CTRL + ";
       break;
    default:
       break;
   }

    int k = ev->key();//普通按键按下
    switch(k)
    {
    case Qt::Key_5:
        str += "key_5";
        break;
    case Qt::Key_A:
        str += "key_A";
        break;
    case Qt::Key_Up:
        str += "key_up";
        break;
    default:
        break;
    }
    ui->label_2->setText(str);
}

void Widget::resizeEvent(QResizeEvent *ev)//窗口被鼠标拖拽移动大小事件
{
    qDebug()<<"old size = "<<ev->oldSize()//老窗口
            <<"current size = "<<ev->size();//新窗口
}
