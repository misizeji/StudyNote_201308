#ifndef CHESSBOARD_H
#define CHESSBOARD_H

// ��������Ҫ��ͷ�ļ�
#include <QtGui/QWidget>
#include <QPen>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

// ������
class ChessBoard : public QWidget
{
    Q_OBJECT

public:
    // ���캯��
    // fileNameΪ���̱���ͼ��·��
    // ����������8*8������Ĭ�ϲ���Ϊ8
    ChessBoard(QWidget *parent = 0 ,
               QString fileName = "",
               int num = 8);
    ~ChessBoard();  // ��������

    // ö�ٱ���
    // Empty��־Ϊ�գ�Black��־Ϊ���ӣ�White��־Ϊ����
    enum GridState{Empty,Black, White };

    int getGrid();                  // �������һ��С����Ŀ��(��߶�)
    void setPen(const QPen & pen);  // ���û���

    // ��������״̬��manInfoΪ������׵�ַ��(px,py)Ϊ��ǰ���ӵ�����
    void setChessManInfo(void *manInfo,int px = -1,int py = -1);

protected:
    // ��ͼ�¼�����Ҫ�����ǻ����̣�����
    void paintEvent(QPaintEvent *);

    // resize�¼������ı䴰��ʱ�Զ�����
    // �˺��������ǻ�ô������µĿ�Ⱥ͸߶ȣ�ͨ������ȷ�����̵��������
    void resizeEvent(QResizeEvent *);

    // ������¼�����Ҫ��ȷ�����̷����λ��
    void mousePressEvent(QMouseEvent *);

private:
    int grid;           // ����һ��С����Ŀ��(��߶�),�����Ⱥ͸߶����
    int startx,starty;  // ���̵��������(startx,starty)
    int gridNum;        // ���Ż����ŵĸ�������������Ϊ8*8������Ϊ8

    // (px,py)Ϊ��ǰ���ӵ�����
    int px;
    int py;

    // ����״ָ̬�룬������մ������ڴ�����������״̬
    void *chessManInfo;
    QPen myPen;                 // ���ʶ���
    QString backImageFileName;  // ������������ͼƬ��·��

signals:
    // ������󣬸������ڷ�����ź�
    // QPoint����Ϊ��ǰ���ӵ�����
    void pressPoint(QPoint);
};

#endif // CHESSBOARD_H
