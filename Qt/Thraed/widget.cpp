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

void Widget::on_pushButton_clicked()//主线程开始
{
    static int t = 1;
    ui->label->setText("主线程开始计算......");
    qDebug()<<"主线程开始计算......"<<t;
    int num = ui->spinBox->value();
    for(int i=0;i < num;i++)
    {
        ;
    }
    QString str = QString("主线程计算结束, %1---").arg(t);
    ui->label->setText(str);
    qDebug()<<"主线程计算结束......"<<t;
    t++;
}

void Widget::on_pushButton_2_clicked()//自定义线程进入
{
    static int t = 1;
    QString str = QString("自定义线程开始,%1---").arg(t);
    qDebug()<<"自定义线程开始....."<<t;
    this->thread->setNum(ui->spinBox->value());//传给thread参数设置MyThread中的num
    this->thread->start();//线程开始运行
    t++;
}

void Widget::myslot()//信号处理函数
{
    static int t = 1;
    QString str = QString("自定义线程运行结束，%1---").arg(t);
    ui->label->setText(str);
    t++;
}
