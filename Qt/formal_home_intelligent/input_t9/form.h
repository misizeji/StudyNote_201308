#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "t9widget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <QSound>


namespace Ui {
    class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

private slots:
    //void on_pushButton_4_clicked();
    //void on_pushButton_5_clicked();
    //void on_pushButton_6_clicked();
    //void on_pushButton_clicked();
    //void on_pushButton_3_clicked();
    //void on_pushButton_2_clicked();
    //void on_toolButton_3_clicked();
    //void on_toolButton_2_clicked();
    //void on_toolButton_clicked();
    void update_string(QString);
    void update_string2(QString);
    void backSpaceText();
    void backSpaceText2();
    //void timeSlot();

private:
    Ui::Form *ui;
    T9Widget *put;
    T9Widget *input;
    QString temperature;
    QString humidity;
    QTimer *timer;
    QString buf;
    QString msg_get_buf;
    QString sbuf;
    QSound bells;

    int temperature_max;
    int humidity_max;
    int temperature_min;
    int humidity_min;
    int temperature_max_before;
    int humidity_max_before;
    int temperature_min_before;
    int humidity_min_before;
    int temperature_flag;
    int humidity_flag;
    int led_state;
    int sound_tem_flag;
    int sound_tem_flag_before;
    int sound_hum_flag;
    int sound_hum_flag_before;

protected:

};

#endif // FORM_H
