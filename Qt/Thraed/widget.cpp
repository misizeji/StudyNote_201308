#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QString>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->thread = new MyThread(this);
    connect(thread,SIGNAL(finished()),this,SLOT(myslot()));
    qDebug()<<"widget id = "<<QThread::currentThreadId();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()//���߳̿�ʼ
{
    static int t = 1;
    ui->label->setText("���߳̿�ʼ����......");
    qDebug()<<"���߳̿�ʼ����......"<<t;
    int num = ui->spinBox->value();
    for(int i=0;i < num;i++)
    {
        ;
    }
    QString str = QString("���̼߳������, %1---").arg(t);
    ui->label->setText(str);
    qDebug()<<"���̼߳������......"<<t;
    t++;
}

void Widget::on_pushButton_2_clicked()//�Զ����߳̽���
{
    static int t = 1;
    QString str = QString("�Զ����߳̿�ʼ,%1---").arg(t);
    qDebug()<<"�Զ����߳̿�ʼ....."<<t;
    this->thread->setNum(ui->spinBox->value());//����thread��������MyThread�е�num
    this->thread->start();//�߳̿�ʼ����
    t++;
}

void Widget::myslot()//�źŴ�����
{
    static int t = 1;
    QString str = QString("�Զ����߳����н�����%1---").arg(t);
    ui->label->setText(str);
    t++;
}
