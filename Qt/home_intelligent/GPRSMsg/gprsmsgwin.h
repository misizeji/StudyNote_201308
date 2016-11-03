#ifndef GPRSMSGWIN_H
#define GPRSMSGWIN_H

#include "../t9input/t9widget.h"
#include <QWidget>

namespace Ui {
    class GPRSMsgWin;
}

class GPRSMsgWin : public QWidget
{
    Q_OBJECT

public:
    explicit GPRSMsgWin(QWidget *parent = 0);
    ~GPRSMsgWin();

private:
    Ui::GPRSMsgWin *ui;
    T9Widget *input;        //���뷨���


signals:
    void show_main_menu();  //��ʾ���˵��ź�


private slots:
    void on_Bt_send_clicked();
    void on_Bt_Exit_clicked();
    void EditText(QString);                             //�༭ѡ�п�
    void backSpaceText();                               //�˸���


protected:
    void paintEvent(QPaintEvent *);//���Ʊ���ͼ

};

#endif // GPRSMSGWIN_H
