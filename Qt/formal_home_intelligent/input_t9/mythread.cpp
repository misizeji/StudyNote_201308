#include "mythread.h"
#include <QDebug>
#include <QSound>

extern "C"  //是大写的 C
{
    //#include "key_deal.h"
}

MyThread::MyThread(QObject *parent) :
    QThread(parent),music("../music/firetrucksiren.wav")
{
    /******按键报警标志位********/
    this->ret_key = 0;
    connect(this,SIGNAL(music_play()),this,SLOT(play_music()));
}

void MyThread::run()//自定义线程运行
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

void MyThread::play_music()//播放音乐
{
   music.play();
}
