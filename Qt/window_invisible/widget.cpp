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
    //û�����ӵ�йرգ���С������󻯣�
    //this->setAttribute(Qt::WA_TranslucentBackground);   //����͸��,���ü��Ĳ������ڴ��ڵ�
    //this->setMask(QRegion(0,0,this->width(),this->height(),QRegion::Ellipse));//����Բ
    QPixmap pix;
    pix.load("../image/6.png");
    this->resize(pix.size());
    this->setMask(pix.mask());
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *)//��ͼ�¼�
{
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap("../image/6.png"));

}

void Widget::mousePressEvent(QMouseEvent *ev)
{
    drag = ev->globalPos()-this->frameGeometry().topLeft();//���ڵ��󶥵�����
}

void Widget::mouseMoveEvent(QMouseEvent *ev)
{
    if(ev->buttons() & Qt::LeftButton)
    {
        this->move(ev->globalPos() - drag);//�ƶ�ʱ����ȥ�˵�������봰�����Ͻǵľ���
    }
}
