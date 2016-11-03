#ifndef WIDGET_H
#define WIDGET_H

#define GO   0
#define BACK 1
#define ON   0
#define OFF  1

#include <QWidget>
#include <QStringList>
#include <QTimer>
#include <QSound>
#include <QMouseEvent>
#include "chessbox.h"
#include "startmovie.h"
#include "startwindow.h"

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void setBackGroundName();
    void initChess();
    void copyChess(int [SIZE][SIZE], int [SIZE][SIZE]);
    int getChessNum(int, int);
    void initConnect();
    void changeRole(int );

private:
    Ui::Widget *ui;
    chessBox *box;
    StartMovie *sMovie;
    StartWindow *sWindow;
    QStringList background; //���汳��ͼ·������
    QTimer *timer;
    QTimer *delay;
    QTimer *songTime;
    QTimer *wTime;
    QSound *backsong;
    int chess[SIZE*SIZE*SIZE][SIZE][SIZE];
    int count;  //������ʾ���
    int current;    //��ǰ������ɫ
    int currentPage;    //��ǰ����ҳ��
    int flags[60];  //��¼ÿ������
    int remind[60]; //��¼ÿ����ʾ
    int whiteNum;   //��¼���Ӹ���
    int blackNum;   //��¼���Ӹ���
    int t;  //����ʱ
    int playMode;     //�����Ѷ�
    int lock;  //������
    int wFlag;
    int sizeFlag;
    int sizeChange;
    int maxflag;
    int moveflag;
    int moveW;
    int moveH;
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
signals:
    void pvc();
    void isEnd();
    void backToStart();
private slots:
    void on_quitToolButton_clicked();
    void on_maxToolButton_clicked();
    void on_minToolButton_clicked();
    void on_endButton_clicked();
    void on_giveupButton_clicked();
    void on_checkToolButton_clicked();
    void on_startButton_clicked();
    void doDrawPos(int, int);
    void doTimeout();
    void doSongTimeOut();
    void doBegin(int);
    void doPvc();
    void doDelay();
    void doIsEnd();
    void doWTimeout();
    void on_passButton_clicked();
    void on_gobackButton_clicked();
    void on_switchButton_clicked();
};

#endif // WIDGET_H
