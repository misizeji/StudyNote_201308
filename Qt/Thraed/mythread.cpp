#include "mythread.h"
#include <QDebug>

MyThread::MyThread(QObject *parent) :
    QThread(parent)
{
}

void MyThread::setNum(int n)//自定义接收n给num赋值
{
    this->num = n;
    qDebug()<<"Pthread Num = "<<QThread::currentThreadId();
}

void MyThread::run()//自定义线程运行
{
    qDebug()<<"Pthread Num = "<<QThread::currentThreadId();
    for (int i=0;i <num;i++)
    {
        ;
    }
    qDebug()<<"自定义线程结束";
}
