#ifndef LEDCNTLWIN_H
#define LEDCNTLWIN_H

#include <QWidget>
#include <QPainter>
#include <QTimer>


namespace Ui {
    class LedCntlWin;
}

class LedCntlWin : public QWidget
{
    Q_OBJECT

public:
    explicit LedCntlWin(QWidget *parent = 0);
    ~LedCntlWin();

private:
    Ui::LedCntlWin *ui;
    QTimer *timer;      //��ʱ��

    int led_state;      //��״̬
    bool IsLed1;        //led1״̬��
    bool Isled2;        //led2״̬��
    bool Isled3;        //led3״̬��


signals:
    void show_main_menu();  //��ʾ���˵��ź�

private slots:
    void TimeroutSlot();           //��ʱ��������
    void on_Bt_LedAll_clicked();
    void on_Bt_Led3_clicked();
    void on_Bt_Led2_clicked();
    void on_Bt_Led1_clicked();
    void on_Bt_Exit_clicked();

protected:
    void paintEvent(QPaintEvent *);//���Ʊ���ͼ


};

#endif // LEDCNTLWIN_H
