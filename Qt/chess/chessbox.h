#ifndef CHESSBOX_H
#define CHESSBOX_H

#define SIZE 8
#define WHITE 0
#define BLACK 1
#define EMPTY 2

#include <QWidget>

class chessBox : public QWidget
{
    Q_OBJECT
public:
    explicit chessBox(QWidget *parent = 0);
    void setChess(int[SIZE][SIZE]);
signals:
    void drawPos(int, int);
public slots:

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
private:
    int w;
    int startX;
    int startY;
    int drawX;
    int drawY;
    int chess[SIZE][SIZE];
};

#endif // CHESSBOX_H
