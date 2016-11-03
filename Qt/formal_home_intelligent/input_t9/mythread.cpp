#include "mythread.h"
#include <QDebug>
#include <QSound>

extern "C"  //�Ǵ�д�� C
{
    //#include "key_deal.h"
}

MyThread::MyThread(QObject *parent) :
    QThread(parent),music("../music/firetrucksiren.wav")
{
    /******����������־λ********/
    this->ret_key = 0;
    connect(this,SIGNAL(music_play()),this,SLOT(play_music()));
}

void MyThread::run()//�Զ����߳�����
{
    qDebug()<<"Pthread Num = "<<QThread::currentThreadId();
    while(1)
   {
       //ret_key = key_deal();
       if(ret_key == 3)
       {
           emit music_play();
       }
    }
}

void MyThread::play_music()//��������
{
   music.play();
}
