#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QDebug>
#include <QString>
#include <QPalette>
#include <QStringList>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QMessageBox>


mainWidget::mainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainWidget)
{
    ui->setupUi(this);


    //������ͼƬ(��ʼ����ͼƬ������)
    this->pic_bg = ":/pic/pic_bg/3.png";

    //���ö�ʱ��
    this->timer = new QTimer(this);
    show_pic_delay = 5;
    this->timer->start(show_pic_delay * 1000);

    //��ȡ����ͼƬ����
    this->pic_bg_list<<":/pic/pic_bg/0.png"<<":/pic/pic_bg/1.png"<<":/pic/pic_bg/2.png"
            <<":/pic/pic_bg/3.png"<<":/pic/pic_bg/4.png"<<":/pic/pic_bg/5.png";

    //ȥ���߿�
    setWindowFlags(Qt::FramelessWindowHint);
    //this->showFullScreen();

    //��ӵ�¼����
    this->login_form = new LoginForm;
    //this->login_form->setGeometry(this->geometry());

    //�ź����
    connect(this->timer,SIGNAL(timeout()),this,SLOT(pic_bg_show()));//����ͼƬ�Զ��л�
    connect(this->login_form,SIGNAL(bg_show()),this,SLOT(widget_show()));//����ͼƬ�����ֲ�

}

mainWidget::~mainWidget()
{
    delete ui;
}


void mainWidget::paintEvent(QPaintEvent *)//��ͼ�¼�������ͼƬ
{
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap(pic_bg));
}

void mainWidget::mousePressEvent(QMouseEvent *ev)//��갴���¼�
{
    int x = ev->x();//�������x����
    int y = ev->y();//�������y����

    qDebug()<<"x = "<<x
            <<" y = "<<y;
    qDebug()<<ev->pos();//��ʾ����pos

    this->timer->stop();

    this->hide();
    this->login_form->show();
    this->login_form->Edit_Clear();
}

void mainWidget::pic_bg_show()//����ͼƬ��ʾ����
{
    static int pic_num = 1;
    int i = (pic_num%5);

    this->pic_bg = pic_bg_list[i];
    pic_num++;
    this->update();//ˢ�»�ͼ
}

void mainWidget::widget_show()//����ͼƬ�����ֲ�
{
    this->show();
    this->timer->start(show_pic_delay * 1000);
}







