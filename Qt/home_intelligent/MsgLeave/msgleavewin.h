#ifndef MSGLEAVEWIN_H
#define MSGLEAVEWIN_H

#include "../t9input/t9widget.h"
#include <QWidget>

namespace Ui {
    class MsgLeaveWin;
}

class MsgLeaveWin : public QWidget
{
    Q_OBJECT

public:
    explicit MsgLeaveWin(QWidget *parent = 0);
    ~MsgLeaveWin();

signals:
    void show_main_menu();  //��ʾ���˵��ź�

private:
    Ui::MsgLeaveWin *ui;
    T9Widget *input;        //���뷨���

private slots:
    void on_Bt_LeaveMsg_clicked();
    void on_Bt_Exit_clicked();
    void EditText(QString);                             //�༭ѡ�п�
    void backSpaceText();                               //�˸���

protected:
    void paintEvent(QPaintEvent *);//���Ʊ���ͼ


};

#endif // MSGLEAVEWIN_H
