#include "msgshowwin.h"
#include "ui_msgshowwin.h"

#include <QTimer>

MsgShowWin::MsgShowWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MsgShowWin)
{
    ui->setupUi(this);

    //È¥³ý±ß¿ò
    setWindowFlags(Qt::FramelessWindowHint);

    //ÉèÖÃ¶¨Ê±Æ÷
    ShowTimer = new QTimer(this);

    connect(ShowTimer,SIGNAL(timeout()),this,SLOT(DoShowMsg()));

}

MsgShowWin::~MsgShowWin()
{
    delete ui;
}

void MsgShowWin::paintEvent(QPaintEvent *)//»­Í¼ÊÂ¼þ»­±³¾°Í¼Æ¬
{
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/pic/pic_bg/bkg.png"));
}

void MsgShowWin::mousePressEvent(QMouseEvent *)
{
    this->ShowTimer->stop();
}

void MsgShowWin::mouseReleaseEvent(QMouseEvent *)
{
    this->ShowTimer->start(100);
}

void MsgShowWin::MsgShow()
{
    /* ¶ÁÈ¡¶ÌÏûÏ¢ */
    this->setWindowOpacity(0);
    this->show();
    this->flag = SHOW;
    this->ShowTimer->start(100);
}

void MsgShowWin::DoShowMsg()
{
    static double tS = 0.00;
    static double tH = 1.00;
    if(flag == SHOW)
    {
        if(tS > 1)
        {
            this->ShowTimer->stop();
            tS = 0.0;
            flag = HIDE;
            this->ShowTimer->start(100);
            return;
        }
        this->setWindowOpacity(tS);
        tS += 0.05;
    }
    else if(flag == HIDE)
    {
        if(tH < 0)
        {
            this->ShowTimer->stop();
            tH = 1.0;
            this->hide();
            emit this->mainmenu_show();
            return;
        }
        this->setWindowOpacity(tH);
        tH -= 0.05;
    }
}
