#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QRegion>
#include <QPixmap>
#include <QBitmap>
#include <QMouseEvent>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Flower");
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowSystemMenuHint
                         |Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint);
    //没有外框，拥有关闭，最小化与最大化；
    //this->setAttribute(Qt::WA_TranslucentBackground);   //背景透明,看得见的才是属于窗口的
    //this->setMask(QRegion(0,0,this->width(),this->height(),QRegion::Ellipse));//画椭圆
    QPixmap pix;
    pix.load("../image/6.png");
    this->resize(pix.size());
    this->setMask(pix.mask());
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *)//画图事件
{
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap("../image/6.png"));

}

void Widget::mousePressEvent(QMouseEvent *ev)
{
    drag = ev->globalPos()-this->frameGeometry().topLeft();//窗口的左顶点坐标
}

void Widget::mouseMoveEvent(QMouseEvent *ev)
{
    if(ev->buttons() & Qt::LeftButton)
    {
        this->move(ev->globalPos() - drag);//移动时，减去了点击处距离窗口左上角的距离
    }
}
