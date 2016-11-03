#ifndef MOVIEWIN_H
#define MOVIEWIN_H

#include <QWidget>
#include <QProcess>
#include <unistd.h>
#include <QString>
#include <QStringList>
#include <QPainter>
#include <QDebug>
#include <QFileDialog>
#include <QMouseEvent>
#include <QTimer>
#include <QTime>

namespace Ui {
    class moviewin;
}

class moviewin : public QWidget
{
    Q_OBJECT

public:
    explicit moviewin(QWidget *parent = 0);
    ~moviewin();
    QTime int_to_time(int);

private:
    Ui::moviewin *ui;
    QProcess *process;      //���linuxϵͳ�������
    QString fileName;       //������Ƶ�ļ���
    QTimer *timer;          //��ʱ��

    bool isPlay;	//���ű�־λ
    bool isStop;	//ֹͣ��־λ
    bool isDoubleClick; //�Ƿ�Ϊ���˫���¼�

    int pos_x_begin;	//������ʼ����x
    int pos_x_end;	//̧���������x
    int pos_y_begin;	//������ʼ����y
    int pos_y_end;	//̧���������y

    int cur_value;  //�洢����ֵ
    int cur_time;	//��ǰ����ʱ��
    int file_length;	//�ļ��ܳ���

private slots:

    void movie_volume_set(int value,bool flag);
    void movie_forward_slots();
    void movie_PlayPause_slots();
    void movie_backward_slots();
    void movie_stop_slots();
    void movie_return_slots();
	void mouse_deal_slots();
    void write_ask_command();
    void get_mplayer_msg();


protected:
    void mousePressEvent(QMouseEvent *);        //������¼�
    void mouseReleaseEvent(QMouseEvent *ev);    //���̧���¼�
    void mouseDoubleClickEvent(QMouseEvent *ev);//���˫���¼�
};

#endif // MOVIEWIN_H
