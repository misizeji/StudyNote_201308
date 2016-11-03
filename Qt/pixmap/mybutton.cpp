#include "mybutton.h"
#include <QMouseEvent>
#include <QTime>

MyButton::MyButton(QWidget *parent):
        QPushButton(parent)
{
    qsrand(QTime(0,0,0).secsTo(QTime(QTime::currentTime())));
    this->setMouseTracking(true);
}

void MyButton::mouseMoveEvent(QMouseEvent *e)
{
    QWidget *p = this->parentWidget();
    if(p == NULL)
    {
        return;
    }
    int pw = p->width();
    int ph = p->height();
    int w = this->width();
    int h = this->height();

    int x = qrand()%(pw - w);
    int y = qrand()%(ph - h);

    this->move(x,y);
}
