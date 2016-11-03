#include "startwindow.h"
#include "ui_startwindow.h"
#include <QPainter>
#include <QSound>
#include <QTimer>
#include <QDebug>

StartWindow::StartWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    this->setWindowOpacity(0);
    windowTime = new QTimer(this);
    this->connect(windowTime,SIGNAL(timeout()),this,SLOT(doWindowTime()));
    flag = SHOW;
    sizeFlag = 100;

    this->setWindowFlags(Qt::FramelessWindowHint   //取消窗口边框
                         |Qt::WindowSystemMenuHint  //设置任务栏右键选项
                         |Qt::WindowMaximizeButtonHint  //设置最大化按钮
                         |Qt::WindowMinimizeButtonHint);    //设置最小化按钮

    this->setGeometry(180,80,1000,600);
    ui->selectWidget->hide();
    ui->aboutWidget->hide();
    ti = new QTimer(this);
    this->connect(ti,SIGNAL(timeout()),this,SLOT(doTimesUp()));

    song = new QSound("./sound/startsound.wav");
    this->setMouseTracking(true);


    maxflag = 0;
}

StartWindow::~StartWindow()
{
    delete ui;
    delete song;
}

void StartWindow::doMovieStop()
{
    this->setWindowOpacity(0);
    this->show();
    flag = SHOW;
    windowTime->start(100);
}

void StartWindow::mouseDoubleClickEvent(QMouseEvent *ev)
{
    if(ev->buttons()&Qt::LeftButton)
    {
        if( (ev->x() < this->width()-95) && (ev->y() > 9) && (ev->y() < 39))
        {
            if(maxflag == 0)
            {
                this->showFullScreen();
                ui->maxToolButton->setIcon(QIcon(":/new/prefix1/max1.png"));
                maxflag = 1;
            }
            else if(maxflag == 1)
            {
                this->showNormal();
                ui->maxToolButton->setIcon(QIcon(":/new/prefix1/tomax1.png"));
                maxflag = 0;
            }
        }
    }
}

void StartWindow::mousePressEvent(QMouseEvent *ev)
{
    if(ev->buttons()&Qt::LeftButton)
    {
        if( (ev->x() < this->width()-95) && (ev->y() > 9) && (ev->y() < 39))
        {
            moveflag = 0;
        }
        else
        {
            moveflag = 1;
        }
        moveW = ev->globalX()-this->x();
        moveH = ev->globalY()-this->y();
        sizeFlag = SIZE_ON;
    }
}
void StartWindow::mouseReleaseEvent(QMouseEvent *ev)
{
    sizeFlag = SIZE_OFF;
}
void StartWindow::mouseMoveEvent(QMouseEvent *ev)
{
    int x = ev->x();
    int y = ev->y();
    int startX = this->x();
    int startY = this->y();
    int ww = this->width();
    int hh = this->height();
    int minW = this->minimumWidth();
    int minH = this->minimumHeight();
    int moveX = ev->globalX();
    int moveY = ev->globalY();


    if(sizeFlag == SIZE_ON && moveflag == 0)
    {
        if(this->isFullScreen() == false)
        {
            this->move(moveX-moveW,moveY-moveH);
        }
        return;
    }
    if(this->isFullScreen())
    {
        return;
    }

    if(sizeFlag != SIZE_ON)
    {
        if( ((x >= 0 && x <= 4) && (y >= 0 && y <= 4))
            || ((x >= ww-4 && x <= ww) && (y >= hh-4 && y <= hh)) )
        {
            this->setCursor(QCursor(Qt::SizeFDiagCursor));
            if((x >= 0 && x <= 4) && (y >= 0 && y <= 4))
            {
                sizeChange = SIZE_L_U;
            }
            else if((x >= ww-4 && x <= ww) && (y >= hh-4 && y <= hh))
            {
                sizeChange = SIZE_R_D;
            }

        }
        else if( ((x >= ww-4 && x <= ww) && (y >= 0 && y <= 4))
                || ((x >= 0 && x <= 4) && (y >= hh-4 && y <= hh)) )
        {
            this->setCursor(QCursor(Qt::SizeBDiagCursor));
            if((x >= 0 && x <= 4) && (y >= hh-4 && y <= hh))
            {
                sizeChange = SIZE_L_D;
            }
            else if((x >= ww-4 && x <= ww) && (y >= 0 && y <= 4))
            {
                sizeChange = SIZE_R_U;
            }

        }
        else if( ((x >= 0 && x <= 4) || (x >= ww-4 && x <= ww))
                && (y > 4 && y < hh-4))
        {
            this->setCursor(QCursor(Qt::SizeHorCursor));
            if(x >= 0 && x <= 4)
            {
                sizeChange = SIZE_LEFT;
            }
            else if(x >= ww-4 && x <= ww)
            {
                sizeChange = SIZE_RIGHT;
            }

        }
        else if( ((y >= 0 && y <= 4) || (y >= hh-4 && y <= hh))
                && (x > 4 && x < ww-4))
        {
            this->setCursor(QCursor(Qt::SizeVerCursor));
            if(y >= 0 && y <= 4)
            {
                sizeChange = SIZE_UP;
            }
            else if(y >= hh-4 && y <= hh)
            {
                sizeChange = SIZE_DONW;
            }
        }
        else
        {
            this->setCursor(QCursor(Qt::ArrowCursor));
        }
    }
    else if(sizeFlag == SIZE_ON)
    {
        if(sizeChange == SIZE_UP)
        {
            int newH = this->height()-(moveY-startY);
            if( (ww <= minW|| hh <= minH) &&(moveY-startY > 0))
            {
                return;
            }
            this->setGeometry(startX,moveY,this->width(),newH);
        }
        else if(sizeChange == SIZE_LEFT)
        {
            int newW = this->width()-(moveX-startX);
            if( (ww <= minW|| hh <= minH) &&(moveX-startX > 0))
            {
                return;
            }
            this->setGeometry(moveX,startY,newW,this->height());
        }
        else if(sizeChange == SIZE_RIGHT)
        {
            int newW = moveX-startX;
            this->resize(newW,this->height());
        }
        else if(sizeChange == SIZE_DONW)
        {
            int newH = moveY-startY;
            this->resize(this->width(),newH);
        }
        else if(sizeChange == SIZE_L_U)
        {
            int newW = this->width()-(moveX-startX);
            int newH = this->height()-(moveY-startY);
            if( (ww <= minW|| hh <= minH) &&(moveY-startY > 0) && (moveX-startX > 0))
            {
                return;
            }
            this->setGeometry(moveX,moveY,newW,newH);
        }
        else if(sizeChange == SIZE_L_D)
        {
            int newW = this->width()-(moveX-startX);
            int newH = moveY-startY;
            if( (ww <= minW|| hh <= minH) &&(moveX-startX > 0))
            {
                return;
            }
            this->setGeometry(moveX,startY,newW,newH);
        }
        else if(sizeChange == SIZE_R_U)
        {
            int newW = moveX-startX;
            int newH = this->height()-(moveY-startY);
            if( (ww <= minW|| hh <= minH) &&(moveY-startY > 0))
            {
                return;
            }
            this->setGeometry(startX,moveY,newW,newH);
        }
        else if(sizeChange == SIZE_R_D)
        {
            int newW = moveX-startX;
            int newH = moveY-startY;
            this->resize(newW,newH);
        }
    }
}

void StartWindow::doWindowTime()
{
    static double tS = 0.0;
    static double tH = 1.0;
    if(flag == SHOW)
    {
        if(tS > 1)
        { 
            windowTime->stop();
            song->play();
            ti->start(95000);
            tS = 0.0;
            return;
        }
        this->setWindowOpacity(tS);
        tS += 0.1;
    }
    else if(flag == HIDE)
    {
        if(tH < 0)
        {
            windowTime->stop();
            tH = 1.0;
            this->hide();
            flag = SHOW;
            return;
        }
        this->setWindowOpacity(tH);
        tH -= 0.1;
    }
}

void StartWindow::doTimesUp()
{
    song->stop();
    song->play();
    ti->start(95000);
}

void StartWindow::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap("./background/margin.png"));
    p.drawPixmap(16,16,this->width()-32,this->height()-32,QPixmap("./background/12.jpg"));
}

void StartWindow::on_startToolButton_clicked()
{
    ui->startWidget->hide();
    ui->modeWidget->hide();
    ui->selectWidget->show();
}

void StartWindow::on_aboutToolButton_clicked()
{
    ui->startWidget->hide();
    ui->aboutWidget->show();
}

void StartWindow::on_backToolButton1_clicked()
{
    ui->selectWidget->hide();
    ui->startWidget->show();
}

void StartWindow::on_backToolButton2_clicked()
{
    ui->aboutWidget->hide();
    ui->startWidget->show();
}

void StartWindow::on_extiToolButton_clicked()
{
    this->close();
}


void StartWindow::on_pvcToolButton_clicked()
{
    ui->backToolButton1->hide();
    ui->pvpToolButton->hide();
    ui->pvcToolButton->hide();
    ui->modeWidget->show();
}

void StartWindow::on_cancelToolButton_clicked()
{
    ui->backToolButton1->show();
    ui->pvcToolButton->show();
    ui->pvpToolButton->show();
    ui->modeWidget->hide();
}

void StartWindow::on_easyToolButton_clicked()
{
    mode = EASY;
    song->stop();
    ti->stop();
    flag = HIDE;
    windowTime->start();
    emit begin(mode);
}

void StartWindow::on_normalToolButton_clicked()
{
    mode = NORMAL;
    song->stop();
    ti->stop();
    flag = HIDE;
    windowTime->start();
    emit begin(mode);
}

void StartWindow::on_hardToolButton_clicked()
{
    mode = HARD;
    song->stop();
    ti->stop();
    flag = HIDE;
    windowTime->start();
    emit begin(mode);
}

void StartWindow::on_pvpToolButton_clicked()
{
    mode = PVP;
    song->stop();
    ti->stop();
    flag = HIDE;
    windowTime->start();
    emit begin(mode);
}

void StartWindow::on_minToolButton_clicked()
{
    this->showMinimized();
}

void StartWindow::on_maxToolButton_clicked()
{
    if(maxflag == 0)
    {
        this->showFullScreen();
        ui->maxToolButton->setIcon(QIcon(":/new/prefix1/max1.png"));
        maxflag = 1;
    }
    else if(maxflag == 1)
    {
        this->showNormal();
        ui->maxToolButton->setIcon(QIcon(":/new/prefix1/tomax1.png"));
        maxflag = 0;
    }
}

void StartWindow::on_quitToolButton_clicked()
{
    this->close();
}

void StartWindow::on_movieToolButton_clicked()
{
    int X = this->x();
    int Y = this->y();
    int W = this->width();
    int H = this->height();
    song->stop();
    ti->stop();
    this->hide();
    emit playMovie(X,Y,W,H);
}
