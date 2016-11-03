#include "widget.h"
#include "ui_widget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QColor>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //button = new MyButton(this);
    //button->setText("click me ^_^");
    this->startX = 0;
    x = -1;
    y = -1;
    this->setCursor(QCursor(QPixmap("../image/wright.png").scaled(30,30)));//设置鼠标图案
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *ev)//画图事件
{
    qDebug()<<"in painEvent"
            <<ev->rect();
    QPainter p(this);//设置画家
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap("../image/ttt.jpg"));//设置起始坐标与图案大小，画图
    p.setPen(Qt::red);//设置画家画笔颜色
    p.setPen(Qt::DashDotDotLine);//设置画家画虚线

    QPen pen;	//新建画笔
    pen.setColor(Qt::red);//设置画笔颜色
    pen.setStyle(Qt::DashDotDotLine);//设置画笔画虚线
    pen.setWidth(3);

    p.setPen(pen);//设置画家拿pen这个画笔
    p.drawLine(50,20,250,20);//画直线

    pen.setColor(Qt::blue);//设置画笔颜色蓝色
    pen.setStyle(Qt::SolidLine);//画线为实线

    p.setPen(pen);//重新设置画家的画笔
    //p.setBrush(Qt::DiagCrossPattern);
    p.setBrush(QBrush(Qt::magenta,Qt::DiagCrossPattern));//设置画刷颜色，画刷样式
    p.drawRect(40,30,200,120);

    p.setPen(QPen());//设置画笔
    p.setBrush(QBrush(QPixmap("../image/test.jpg")));//设置画的目标图案
    p.drawEllipse(260,40,150,200);//画在一个椭圆中

    qDebug()<<"startX = "<<this->startX;
    p.drawPixmap(startX,50,80,80,QPixmap("../image/face-1.png"));//drawpixmap事件画图

    if(x != -1 && y >= 0)
    p.drawPixmap(x-20,y-20,50,50,QPixmap("../image/myheart.png"));//鼠标点哪，就在哪画图，从中心开始画图

    p.setPen(Qt::green);//画笔绿色
    p.setFont(QFont("songti",20));//画的字体
    p.drawText(200,250,"hello Qt");//画的文本内容
}

void Widget::on_pushButton_clicked()//按键按下移动图片
{
    startX += 50;
    if(startX >= this->width())
    {
        startX = 0;
    }
    this->update();
}

void Widget::mousePressEvent(QMouseEvent *ev)//鼠标点击事件记录x，y坐标
{
    x = ev->x();
    y = ev->y();
    update();
}
