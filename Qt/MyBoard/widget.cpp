#include "widget.h"
#include "ui_widget.h"

#include <QPixmap>
#include <QPainter>
#include <QBitmap>
#include <QPaintEvent>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("My Board");
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowSystemMenuHint
                         |Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint);
    //没有外框，拥有关闭，最小化与最大化；
    QPixmap pix;
    pix.load("../blackwhite_chess/lines.png");
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
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap("../blackwhite_chess/lines.png"));
}

