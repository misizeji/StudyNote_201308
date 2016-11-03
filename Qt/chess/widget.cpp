#include "widget.h"
#include "ui_widget.h"
#include "myjudge.h"
#include <QPaintEvent>
#include <QDebug>
#include <QString>
#include <QPainter>
#include <QTimer>
#include <QTime>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setMouseTracking(true);
    this->setWindowFlags(Qt::FramelessWindowHint    //ȡ�����ڱ߿�
                         |Qt::WindowSystemMenuHint  //�����������Ҽ�ѡ��
                         |Qt::WindowMaximizeButtonHint  //������󻯰�ť
                         |Qt::WindowMinimizeButtonHint);    //������С����ť

    delay = new QTimer(this);   //����������ʱ
    timer = new QTimer(this);   //����ʱ�䵹��ʱ
    songTime = new QTimer(this);    //��������ѭ�����ż�ʱ
    wTime = new QTimer(this);
    backsong = new QSound("./sound/backsong.wav");

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));  //�������
    lock = OFF;

    sMovie = new StartMovie();
    sMovie->show();
    sWindow = new StartWindow;
    ui->endLabel->hide();

    //�������
    box = new chessBox(this);
    ui->gridLayout_2->addWidget(box, 1, 0);
    box->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    initConnect();

    setBackGroundName();    //���ñ���ͼƬ·��

    initChess();    //��ʼ������
    count = 7;  //�״����������
    sizeFlag = 100;
    maxflag = 0;

    t = 30;
    ui->timeLcdNumber->display(t);

}

//�����ź����
void Widget::initConnect()
{
    this-connect(this->box,SIGNAL(drawPos(int,int)),this,SLOT(doDrawPos(int,int)));
    this->connect(this->sWindow,SIGNAL(begin(int)),
            this,SLOT(doBegin(int)));
    this->connect(timer,SIGNAL(timeout()),
            this,SLOT(doTimeout()));
    this->connect(this,SIGNAL(pvc()),this,SLOT(doPvc()));
    this->connect(this,SIGNAL(isEnd()),this,SLOT(doIsEnd()));
    this->connect(delay,SIGNAL(timeout()),
            this,SLOT(doDelay()));
    this->connect(songTime,SIGNAL(timeout()),SLOT(doSongTimeOut()));
    this->connect(wTime,SIGNAL(timeout()),this,SLOT(doWTimeout()));
    this->connect(this,SIGNAL(backToStart()),this->sWindow,SLOT(doWindowTime()));
    this->connect(sMovie,SIGNAL(movieStop()),this->sWindow,SLOT(doMovieStop()));
    this->connect(sWindow,SIGNAL(playMovie(int,int,int,int)),
                    sMovie,SLOT(doPlayMovie(int,int,int,int)));
}

Widget::~Widget()
{
    delete ui;
    delete backsong;
}

void Widget::doWTimeout()
{
    static double tS = 0.0;
    static double tH = 1.0;
    if(wFlag == SHOW)
    {
        if(tS > 1)
        {
            wTime->stop();
            tS = 0.0;
            return;
        }
        this->setWindowOpacity(tS);
        tS += 0.1;
    }
    else if(wFlag == HIDE)
    {
        if(tH < 0)
        {
            wTime->stop();
            tH = 1.0;
            this->hide();
            return;
        }
        this->setWindowOpacity(tH);
        tH -= 0.1;
    }
}

//�ۺ�����ʵ�ֱ�������ѭ������
void Widget::doSongTimeOut()
{
    backsong->stop();
    backsong->play();
    songTime->start(504000);
}

//���ñ���ͼ·������
void Widget::setBackGroundName()
{
    background<<"./background/12.jpg"<<"./background/5.jpg"
              <<"./background/9.jpg"<<"./background/10.jpg"
              <<"./background/11.jpg"<<"./background/14.jpg"
              <<"./background/16.jpg"<<"./background/20.png"
              <<"./background/21.jpg"<<"./background/23.jpg"
              <<"./background/24.jpg"<<"./background/25.jpg"
              <<"./background/28.jpg"<<"./background/30.jpg"
              <<"./background/IceFrog.jpg"<<"./background/33.jpg"
              <<"./background/34.jpg"<<"./background/35.jpg"
              <<"./background/36.jpg"<<"./background/37.jpg"
              <<"./background/42.jpg"<<"./background/43.jpg"
              <<"./background/46.jpg"<<"./background/47.jpg"
              <<"./background/48.jpg"<<"./background/49.jpg"
              <<"./background/50.jpg"<<"./background/51.png"
              <<"./background/52.png"<<"./background/53.png"
              <<"./background/54.png";
}

//��ʼ������
void Widget::initChess()
{
    for(int page=0;page<SIZE*SIZE;page++)
    {
        for(int j=0;j<SIZE;j++)
        {
            for(int i=0;i<SIZE;i++)
            {
                chess[page][j][i] = EMPTY;
            }
        }
    }
    chess[0][3][3] = chess[0][4][4] = BLACK;
    chess[0][3][4] = chess[0][4][3] = WHITE;

    box->setChess(chess[0]);  //�������̽��������� ����
}

//���Ʊ���
void Widget::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap("./background/margin.png"));
    p.drawPixmap(16,16,this->width()-32,this->height()-32,QPixmap(background[count]));
}

//�л�����
void Widget::on_switchButton_clicked()
{
    count++;
    if(count == 31)
    {
        count = 0;
    }
    this->update();
}

//����ʱ�䵹��ʱ
void Widget::doTimeout()
{

    if(t == 0)
    {
        ui->passButton->click();
        t = 30;
        ui->timeLcdNumber->display(t);
    }
    t--;
    ui->timeLcdNumber->display(t);
}


//===========================================//
//����������Ϣ:                                //
//����:int p ����ҳ��                          //
//===========================================//
void Widget::copyChess(int dest[SIZE][SIZE], int src[SIZE][SIZE])
{
    for(int j=0;j<SIZE;j++)
    {
        for(int i=0;i<SIZE;i++)
        {
            dest[i][j] = src[i][j];
        }
    }
}

//============================================//
//�������Ӹ���:                                 //
//����: int p ����ҳ��  int role ��������        //
//============================================//
int Widget::getChessNum(int p, int role)
{
    int num = 0;
    for(int j=0;j<SIZE;j++)
    {
        for(int i=0;i<SIZE;i++)
        {
            if(chess[p][i][j] == role)
            {
                num++;
            }
        }
    }
    return num;
}

//===============================================//
//���ܣ��л����ӽ�ɫ                                //
//����: int flag ��־λ GO��ʾ���ӽ��� BACK��ʾ����   //
//===============================================//
void Widget::changeRole(int flag)
{
    QString pic;
    //��ʾ��ʾ
    if(flag == GO)
    {
         pic = QString(":/new/prefix1/%1.png").arg(current);
    }
    if(flag == BACK)
    {
         pic = QString(":/new/prefix1/%1.png").arg(remind[currentPage]);
    }
    ui->currentlabel->setPixmap(QPixmap(pic));
    this->update();

    //��ʾ˫��������
    whiteNum = this->getChessNum(currentPage, WHITE);
    blackNum = this->getChessNum(currentPage, BLACK);
    ui->whiteLcdNumber->display(whiteNum);
    ui->blackLcdNumber->display(blackNum);
}

//================================================//
//�ۺ���:����������Ϣ                                //
//����:int drawX ������  int drawY������             //
//================================================//
void Widget::doDrawPos(int drawX, int drawY)
{
    if(lock == OFF)
    {
        return;
    }
    this->copyChess(chess[currentPage+1],chess[currentPage]); //����ǰ���̿�������һҳ
    //�����㷨
    int eatNum = myJudgeRule(drawX, drawY,
                             (void *)chess[currentPage+1], current);

    if(eatNum == 0) //����Ϊ0����������Ч
    {
        return;
    }
    currentPage++;

    box->setChess(chess[currentPage]);  //�������̽��������� ����

    flags[currentPage] = current;   //���汾�����ӷ�
    remind[currentPage] = current = !current;   //���汾����ʾ����

    changeRole(GO);
    t = 30;
    ui->timeLcdNumber->display(t);

    emit isEnd();

    if(playMode != PVP) //��Ϸģʽ�ж�
    {
        emit pvc();
        lock = OFF; //������ȷ����������ǰ���ҷ���������
    }
}

//����
void Widget::on_gobackButton_clicked()
{
    //�ص���1���������ٻ�
    if(currentPage == 0)
    {
        return;
    }

    current = flags[currentPage];   //��������
    currentPage--;
    box->setChess(chess[currentPage]);  //�������̽��������� ����
    changeRole(BACK);
    t = 30;
    ui->timeLcdNumber->display(t);
}

//�������ܣ��������ӣ��öԷ���
void Widget::on_passButton_clicked()
{
    remind[currentPage] = current = !current;
    this->copyChess(chess[currentPage+1],chess[currentPage]); //����ǰ���̿�������һҳ
    QString pic = QString(":/new/prefix1/%1.png").arg(remind[currentPage]);
    ui->currentlabel->setPixmap(QPixmap(pic));
    this->update();

    t = 30;
    ui->timeLcdNumber->display(t);
    if(playMode != PVP)
    {
        emit pvc();
    }
}

//==================================================================//
//�ۺ���: �����崰�� �����ô���λ�úʹ�С                              //
//����:   int mode �Ѷ�ģʽ                                          //
//=================================================================//
void Widget::doBegin(int mode)
{
    this->setGeometry(this->sWindow->geometry());
    this->setWindowOpacity(0);
    this->show();
    wFlag = SHOW;
    wTime->start(100);
    backsong->play();
    songTime->start(504000);
    playMode = mode;
    t = 30;
    ui->timeLcdNumber->display(t);
    ui->timeLcdNumber->display(t);
    ui->checkToolButton->setEnabled(false);
    ui->giveupButton->setEnabled(false);
    ui->passButton->setEnabled(false);
    ui->gobackButton->setEnabled(false);
}


//�ۺ���:  �����������λ��
void Widget::doPvc()
{
    int temp[SIZE][SIZE] = {{EMPTY}}; //������
    int norPos[60][2] = {{0}};
    int maxPos[2] = {0};    //������������
    int minPos[2] = {0};    //��С����������
    int eatMin = SIZE*SIZE; //��¼��С������
    int eatMax = 0;         //��¼��������
    int eat = 0;    //ÿ�γ�����
    int x = 0, y = 0;   //������������
    int k = 0;

    for(int j=0;j<SIZE;j++)
    {
        for(int i=0;i<SIZE;i++)
        {
            //����ǰ���̸��Ƹ�������
            this->copyChess(temp,chess[currentPage]);
            //�����㷨��ÿɳ���λ�ã���������
            eat = myJudgeRule(i, j,(void *)temp, current);

            if(eat != 0)
            {
                norPos[k][0] = i;
                norPos[k][1] = j;
                k++;
                if(eat >= eatMax)
                {
                    eatMax = eat;
                    maxPos[0] = i;
                    maxPos[1] = j;
                }
                if(eat <= eatMin)
                {
                    eatMin = eat;
                    minPos[0] = i;
                    minPos[1] = j;
                }
            }
        }
    }

    if(eatMin == SIZE*SIZE)    //���Ҳ����ɳ������꣬����
    {
        ui->passButton->click();
        lock = ON;
        return;
    }

    //����ǰ���̿�������һҳ
    this->copyChess(chess[currentPage+1],chess[currentPage]);

    if(playMode == EASY)
    {
        x = minPos[0];
        y = minPos[1];
    }
    else if(playMode == NORMAL)
    {
        if(k <= 1)
        {
            k = 0;
        }
        else
        {
            k = qrand()%(k-1);
        }
        x = norPos[k][0];
        y = norPos[k][1];
    }
    else if(playMode == HARD)
    {
        x = maxPos[0];
        y = maxPos[1];
    }

    //�����㷨����
    eat = myJudgeRule(x, y,(void *)chess[currentPage+1], current);
    if(eat == 0)
    {
        ui->passButton->click();
        return;
    }
    currentPage++;
    delay->start(1000);
}

//�ۺ�������ʱ��������������
void Widget::doDelay()
{
    box->setChess(chess[currentPage]);  //�������̽��������� ����
    flags[currentPage] = current;   //���汾�����ӷ�
    remind[currentPage] = current = !current;   //���汾����ʾ����
    changeRole(GO); //����ת��
    t = 30;   //���õ���ʱ
    ui->timeLcdNumber->display(t);
    delay->stop();  //�ر���ʱ��
    emit isEnd();
    lock = ON;  //������ϣ�����
}

//�ۺ�������ʼ��Ϸ
void Widget::on_startButton_clicked()
{
    ui->endLabel->setText("");
    whiteNum = 2;
    blackNum = 2;
    ui->whiteLcdNumber->display(whiteNum);
    ui->blackLcdNumber->display(blackNum);

    currentPage = 0;   //��ʼ��ҳ��
    current = WHITE;    //�״�����
    flags[currentPage] = BLACK;
    remind[currentPage] = WHITE;

    initChess();    //��ʼ������

    box->setChess(chess[currentPage]);  //�������̽��������� ����
    this->update();

    ui->endLabel->hide();
    box->show();
    ui->checkToolButton->setEnabled(false);
    ui->giveupButton->setEnabled(true);
    ui->passButton->setEnabled(true);
    ui->gobackButton->setEnabled(true);
    lock = ON;
    timer->start(1000);
    ui->startButton->setEnabled(false);
}

//�ܺ������ж���Ӯ
void Widget::doIsEnd()
{
    if((whiteNum+blackNum) == 64)
    {
        ui->giveupButton->setEnabled(false);
        ui->passButton->setEnabled(false);
        ui->gobackButton->setEnabled(false);
        lock = OFF;
        timer->stop();
        box->hide();
        if(whiteNum > blackNum)
        {
            ui->endLabel->setText("����ʤ����");
        }
        else if(whiteNum < blackNum)
        {
            ui->endLabel->setText("����ʤ����");
        }
        else
        {
            ui->endLabel->setText("ƽ�֣�");
        }
        ui->endLabel->show();
        ui->startButton->setEnabled(true);
    }
    else
    {
        if(whiteNum == 0)
        {
            ui->giveupButton->setEnabled(false);
            ui->passButton->setEnabled(false);
            ui->gobackButton->setEnabled(false);
            lock = OFF;
            timer->stop();
            box->hide();
            ui->endLabel->setText("����ʤ����");
        }
        if(blackNum == 0)
        {
            ui->giveupButton->setEnabled(false);
            ui->passButton->setEnabled(false);
            ui->gobackButton->setEnabled(false);
            lock = OFF;
            timer->stop();
            box->hide();
            ui->endLabel->setText("����ʤ����");
        }
        ui->endLabel->show();
        ui->startButton->setEnabled(true);
        ui->checkToolButton->setEnabled(true);
    }
}

//��Ϸ������鿴������ӷֲ�
void Widget::on_checkToolButton_clicked()
{
    ui->endLabel->hide();
    box->show();
}

//���䰴ť
void Widget::on_giveupButton_clicked()
{
    //�ж����䷽
    if(remind[currentPage] == WHITE)
    {
        whiteNum = 0;
    }
    else
    {
        blackNum = 0;
    }
    emit isEnd();
}

//�˳���ť�����ؿ�ʼ�˵�
void Widget::on_endButton_clicked()
{
    this->sWindow->setGeometry(this->geometry());
    this->sWindow->setWindowOpacity(0);
    this->sWindow->show();
    backsong->stop();
    emit backToStart();
    this->sWindow->windowTime->start();
    wFlag = HIDE;
    wTime->start(100);
    ui->endLabel->hide();
    box->show();
    initChess();    //��ʼ������
    box->setChess(chess[0]);  //�������̽��������� ����
    whiteNum = 2;
    blackNum = 2;
    ui->whiteLcdNumber->display(whiteNum);
    ui->blackLcdNumber->display(blackNum);
    this->update();
    timer->stop();
}

void Widget::mouseDoubleClickEvent(QMouseEvent *ev)
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

void Widget::mousePressEvent(QMouseEvent *ev)
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
void Widget::mouseReleaseEvent(QMouseEvent *ev)
{
    sizeFlag = SIZE_OFF;
}
void Widget::mouseMoveEvent(QMouseEvent *ev)
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

void Widget::on_minToolButton_clicked()
{
    this->showMinimized();
}

void Widget::on_maxToolButton_clicked()
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

void Widget::on_quitToolButton_clicked()
{
    this->close();
}
