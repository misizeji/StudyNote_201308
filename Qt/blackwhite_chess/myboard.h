#ifndef MYBOARD_H
#define MYBOARD_H

#include <QWidget>
#include <QtNetwork>
#include "mylink.h"//�������ͷ�ļ�

namespace Ui {
    class MyBoard;
}

class MyBoard : public QWidget
{
    Q_OBJECT

public:
    explicit MyBoard(QWidget *parent = 0);
    ~MyBoard();
    int mouse_flag;    //���������Ӧ��־λ
    int get_count(int);//������������
    int put_times;   //��ǰ����Ĵ���
    int Player_flag; //ѡ�ֱ�־(ͬʱҲ��������Ҫ�洢������)
    int my_flag;     //�����ս��¼�Լ��ı�־λ
    int play_mode;   //��Ϸģʽ(0����δ��ʼ��1�������˶�ս��2�����˻���ս��3���������ս)
    void pvc_game_mode(int x,int y);    //�˻���ս������
    void pvp_game_mode(int x,int y);    //���˶�սģʽ������
    void pvnet_game_mode(int x,int y);  //�����ս����
    void link_pvnet();                  //�������Ӻ���
    void game_init();   //��Ϸ���ݳ�ʼ��
    void refresh();//�Զ��طź���
    QString put_tips();//��ʾ���ӣ�һ��λ�ã�


public slots:
    void set_put_before(int);  //�ָ�����
    void player_flag_change();//����ѡ�ֱ�־
    void timer_deal();         //�������崦����
    void refresh_deal();       //�طŴ�����
    void msg_send(int x,int y,int flag);//����������Ϣ����
    void game_begin();         //�����ս��ʼ
    void processPendingDatagram();//��������������

private:
    Ui::MyBoard *ui;
    QUdpSocket *sender;//�ͻ���
    QUdpSocket *receiver;//������
    QTimer *timer;          //�������嵹��ʱ
    QTimer *refresh_timer;  //�Զ��طŵ���ʱ
    mylink *link_win;   //�����ս��Ϣ����
    int count;              //���������
    int chess_put[8][8];    //��������
    int chess_put_temp[8][8];   //���Ӻ����ж��м��������
    int chess_put_before[61][8][8];//���������ά����
    QString chess_pic_my;      //�Լ�����ͼƬ
    QString chess_pic_other;   //��������ͼƬ
    int a,b;                   //�������µ�����
    int t;                  //��ʱ����
    int put_times_before;   //��¼�������
    int times_temp;         //�ط��м�ѭ������


signals:
    void update_signal();   //ˢ�¼Ƿְ��ź�
    void update_time_signal();  //ˢ��ʱ���ź�
    void game_over();           //��Ϸ�����ź�

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    bool iseated();    //�жϳ��Ӻ���
    int my_judge(int x,int y,int gridNum, void *chess_array_addr, int currentRole); //�㷨

};

#endif // MYBOARD_H
