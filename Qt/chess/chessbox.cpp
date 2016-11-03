#include "chessbox.h"
#include <QPaintEvent>
#include <QDebug>
#include <QString>
#include <QPainter>
#include <QPen>

chessBox::chessBox(QWidget *parent) :
    QWidget(parent)
{
}

//===============================================//
//����:����������Ϣ                                //
//����:c[SIZE][SIZE] ������Ϣ                     //
//==============================================//
void chessBox::setChess(int c[SIZE][SIZE])
{
    for(int j=0;j<SIZE;j++)
    {
        for(int i=0;i<SIZE;i++)
        {
            chess[j][i] = c[j][i];
        }
    }
}

//��ͼ�¼����������̣���������
void chessBox::paintEvent(QPaintEvent *ev)
{
    //ȷ��������
    if(this->width() <= this->height())
    {
        w = this->width()/8;
    }
    if(this->width() > this->height())
    {
        w = this->height()/8;
    }

    //ȷ����ʼ����
    startX = this->width()/2 - (SIZE/2)*w;
    startY = this->height()/2 - (SIZE/2)*w;

    QPainter p(this);
    QPen pen;
    pen.setColor(Qt::blue);
    pen.setWidth(2);
    p.setPen(pen);

    //��������
    for(int j=0;j<SIZE;j++)
    {
        for(int i=0;i<SIZE;i++)
        {
            p.drawRect(startX+i*w, startY+j*w, w, w);
        }
    }

    //��ָ��λ�û�������
    for(int j=0;j<SIZE;j++)
    {
        for(int i=0;i<SIZE;i++)
        {
            if(chess[i][j] == WHITE)
            {
                p.drawPixmap(startX+i*w, startY+j*w, w, w,
                             QPixmap(":/new/prefix1/0.png"));
            }
            if(chess[i][j] == BLACK)
            {
                p.drawPixmap(startX+i*w, startY+j*w, w, w,
                             QPixmap(":/new/prefix1/1.png"));
            }
        }
    }
}

//������¼�
void chessBox::mousePressEvent(QMouseEvent *ev)
{
    //������������������������
    int drawX = (ev->x()-startX)/w;
    int drawY = (ev->y()-startY)/w;
    int button = ev->button();
    //��������������ڣ�������Ч
    if(ev->x()<startX || ev->x()>startX+w*SIZE || ev->y()<startY || ev->y()>startY+w*SIZE)
    {
        return;
    }
    if(button == Qt::LeftButton)
    {
        emit drawPos(drawX, drawY);
    }
}
