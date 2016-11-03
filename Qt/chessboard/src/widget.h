#ifndef WIDGET_H
#define WIDGET_H


#include <QWidget>
#include <QTimer>
#include <QPoint>
#include <QMouseEvent>
#include <QDebug>
#include <QPen>
#include <QPainter>
#include <QMessageBox>
#include "chessboard.h"

namespace Ui {
    class Widget;
}

// ������
class Widget : public QWidget {
    Q_OBJECT
public:
    Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    // �ۺ������������̴��ڵ��ź�
    // ����Ϊ�������������
    void doPressPoint(QPoint);

protected:
    // ��ͼ�¼�����Ҫ�ǻ���Ȼ���ڵı���
    void paintEvent(QPaintEvent *);

private:

    Ui::Widget *ui;
    ChessBoard *board;  // ������Ķ���
    bool isStart;
    bool PVPFlag;       // ���˶�ս�ı�־λ
    ChessBoard::GridState currentRole;  // ö�ٱ���������״̬�ı�־λ
    int chess[64][8][8];    // ��ά���飬�����־����״̬
    int step;               // ��־��ǰ����Ϊ�ڼ���
    int gridNum;
    int times;
    int failFlag;
    QTimer timer;           // ����ʱ
    QTimer machineTime;     // ������ʱ�䣬��ʱ����
    void showScore();       // ��ʾ����(�ڰ��Ӹ��Ե���Ŀ)
    void changeRole();      // �ı��ɫ

    // ���ӵĹ���
    // ���ӹ���Ĳ�����������������λ��(x y)�����Ż����ŵĸ���(gridNum)��
    // ����״̬(chess)�����ӵ�����Ȩ(ע���������д��)��eatChessĬ��Ϊtrue
    int judgeRule(int, int, int , void *, ChessBoard::GridState,bool eatChess=true);
    void init();    // ��ʼ��

private slots:
    void on_toolButtonOver_clicked();   // ��Ϸ������ť
    void on_toolButtonJump_clicked();   // ������ť
    void on_toolButtonGoback_clicked(); // ���尴ť
    void on_toolButtonMode_clicked();   // ģʽ(���˻�����)ѡ��ť
    void on_toolButtonStart_clicked();  // ������ť
    void timerSlot();   // ����ʱ�Ĳۺ���
    void PVEmethod();   // �����Ĳۺ���
};

#endif // WIDGET_H
