#ifndef MSGSHOWWIN_H
#define MSGSHOWWIN_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>

#define SHOW 1
#define HIDE 0

namespace Ui {
    class MsgShowWin;
}

class MsgShowWin : public QWidget
{
    Q_OBJECT

public:
    explicit MsgShowWin(QWidget *parent = 0);
    ~MsgShowWin();

private:
    Ui::MsgShowWin *ui;
    QTimer *ShowTimer;
    int flag;

signals:
    void mainmenu_show();

protected:
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);        //������¼�
    void mouseReleaseEvent(QMouseEvent *ev);    //���̧���¼�

private slots:
    void DoShowMsg();

public slots:
    void MsgShow();

};

#endif // MSGSHOWWIN_H
