#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <phonon/VideoPlayer>
#include "myplayer.h"       //����������ͷ�ļ�
#include "myboard.h"        //��������ͷ�ļ�
#include "udp_talk.h"       //������촰��

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    MyPlayer *Player;//��Ƶ������
    MyBoard *board; //����
    udp_talk *mytalk;//����
    QTimer *timer;  //��ʱ��
    Phonon::VideoPlayer *music_player;//�������ֲ�����
    QString pic_name;//����ͼƬ����
    int play_time;  //����ʱ����
    int music_player_flag;//�������ֲ�������־λ

private slots:
    void on_toolButton_talk_clicked();//���촰��
    void on_toolButton_tips_clicked();//��ʾ
    void on_toolButton_clicked();//�Զ��ط�
    void on_toolButton_music_clicked();//�������ְ�ť
    void on_toolButton_Regret_clicked();//����
    void on_toolButton_pvnet_clicked();//�����ս
    void on_toolButton_PVC_clicked();//�˻���ս
    void on_toolButton_PVP_clicked();//���˶�ս
    void on_WatchMovie_clicked();//�ۿ���Ӱ
    void on_button_ChangeBG_clicked();//�޸ı���
    void update_score();    //���·���
    void update_time();     //���µ���ʱ
    void lcd_time_show();   //��ʾ����ʱ
    void game_over_judge(); //��Ϸ�����ж�

protected:
    void paintEvent(QPaintEvent *);
    void set_time_count();      //���ö�ʱ������
};

#endif // WIDGET_H
