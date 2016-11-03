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
//功能:接收棋盘信息                                //
//参数:c[SIZE][SIZE] 棋盘信息                     //
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

//绘图事件，绘制棋盘，绘制落子
void chessBox::paintEvent(QPaintEvent *ev)
{
    //确定网格宽度
    if(this->width() <= this->height())
    {
        w = this->width()/8;
    }
    if(this->width() > this->height())
    {
        w = this->height()/8;
    }

    //确定起始坐标
    startX = this->width()/2 - (SIZE/2)*w;
    startY = this->height()/2 - (SIZE/2)*w;

    QPainter p(this);
    QPen pen;
    pen.setColor(Qt::blue);
    pen.setWidth(2);
    p.setPen(pen);

    //绘制棋盘
    for(int j=0;j<SIZE;j++)
    {
        for(int i=0;i<SIZE;i++)
        {
            p.drawRect(startX+i*w, startY+j*w, w, w);
        }
    }

    //在指定位置绘制棋子
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

//鼠标点击事件
void chessBox::mousePressEvent(QMouseEvent *ev)
{
    //根据鼠标点击坐标计算网格坐标
    int drawX = (ev->x()-startX)/w;
    int drawY = (ev->y()-startY)/w;
    int button = ev->button();
    //点击坐标在棋盘内，才算有效
    if(ev->x()<startX || ev->x()>startX+w*SIZE || ev->y()<startY || ev->y()>startY+w*SIZE)
    {
        return;
    }
    if(button == Qt::LeftButton)
    {
        emit drawPos(drawX, drawY);
    }
}
