#include "startmovie.h"
#include "ui_startmovie.h"
#include "Phonon/VideoPlayer"
#include <QKeyEvent>
#include <QPainter>


StartMovie::StartMovie(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartMovie)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint   //取消窗口边框
                         |Qt::WindowSystemMenuHint  //设置任务栏右键选项
                         |Qt::WindowMaximizeButtonHint  //设置最大化按钮
                         |Qt::WindowMinimizeButtonHint);    //设置最小化按钮
    this->setGeometry(180,80,1000,600);
    start= new Phonon::VideoPlayer(Phonon::VideoCategory, this);
    connect(this->start,SIGNAL(finished()),this,SLOT(doFinish()));
    start->setGeometry(16,16,this->width()-32,this->height()-32);
    start->play(Phonon::MediaSource("./video/startmovie.flv"));

    start->play();
}

StartMovie::~StartMovie()
{
    delete ui;
}

void StartMovie::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap("./background/margin.png"));
}

void StartMovie::doPlayMovie(int X, int Y, int W, int H)
{
    this->setGeometry(X,Y,W,H);
    start->setGeometry(16,16,this->width()-32,this->height()-32);
    this->show();
    start->play();
}

void StartMovie::doFinish()
{
    start->stop();
    emit movieStop();
    this->close();
}

void StartMovie::mouseDoubleClickEvent(QMouseEvent *ev)
{
    static int sizeflag = 0;

    if(ev->buttons()&Qt::LeftButton)
    {
        if(sizeflag == 0)
        {
            this->showFullScreen();
            sizeflag = 1;
        }
        else if(sizeflag == 1)
        {
            this->showNormal();
            sizeflag = 0;
        }
        start->setGeometry(16,16,this->width()-32,this->height()-32);
    }
}

void StartMovie::keyPressEvent(QKeyEvent *ev)
{
    int key = ev->key();
    static int space = 0;
    static float tV = 1.0;
    switch(key)
    {
    case Qt::Key_Escape:
        doFinish();
        break;
    case Qt::Key_Space:
        {
            if(space == 0)
            {
                start->pause();
                space = 1;
            }
            else
            {
                start->play();
                space = 0;
            }
            break;
        }

    case Qt::Key_Up:
        {
            if(tV <= 1.0)
            {
               tV += 0.1;
               start->setVolume(tV);
            }
            break;
        }
    case Qt::Key_Down:
        {
            if(tV >= 0.0)
            {
               tV -= 0.1;
               start->setVolume(tV);
            }
            break;
        }
    case Qt::Key_Right:
        {
            start->seek(start->currentTime()+10*1000);
            break;
        }
    case Qt::Key_Left:
        {
            start->seek(start->currentTime()-10*1000);
            break;
        }

    }
}
