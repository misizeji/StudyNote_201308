#include "mythread.h"
#include <QDebug>

MyThread::MyThread(QObject *parent) :
    QThread(parent)
{
}

void MyThread::setNum(int n)//�Զ������n��num��ֵ
{
    this->num = n;
    qDebug()<<"Pthread Num = "<<QThread::currentThreadId();
}

void MyThread::run()//�Զ����߳�����
{
    qDebug()<<"Pthread Num = "<<QThread::currentThreadId();
    for (int i=0;i <num;i++)
    {
        ;
    }
    qDebug()<<"�Զ����߳̽���";
}
