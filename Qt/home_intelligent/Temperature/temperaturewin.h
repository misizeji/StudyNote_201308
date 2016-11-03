#ifndef TEMPERATUREWIN_H
#define TEMPERATUREWIN_H

#include <QWidget>
#include <QPainter>
#include <QSound>
#include <QTimer>

namespace Ui {
    class TemperatureWin;
}

class TemperatureWin : public QWidget
{
    Q_OBJECT

public:
    explicit TemperatureWin(QWidget *parent = 0);
    ~TemperatureWin();

signals:
    void show_main_menu();  //��ʾ���˵��ź�

private:
    Ui::TemperatureWin *ui;
    QTimer *timer;              //��ʱ��
    QString Temperature_Value;
    QString Humidity_Value;
    QString recv_buf;
    QSound bells;

    int temperature_max;            //�¶�����
    int temperature_min;            //�¶�����
    int humidity_max;               //ʪ������
    int humidity_min;               //ʪ������
    int temperature_max_before;     //�洢�¶�����
    int temperature_min_before;     //�洢�¶�����
    int humidity_max_before;        //�洢ʪ������
    int humidity_min_before;        //�洢ʪ������
    int temperature_flag;           //�¶ȵĳ����־
    int humidity_flag;              //ʪ�ȵĳ����־

    //��ʪ�������־λ
    int sound_tem_flag;
    int sound_tem_flag_before;
    int sound_hum_flag;
    int sound_hum_flag_before;

private slots:
    void on_Bt_Commit_clicked();
    void on_Bt_Exit_clicked();
    void TimeSlot();

protected:
    void paintEvent(QPaintEvent *);//���Ʊ���ͼ


};

#endif // TEMPERATUREWIN_H
