#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include "../MsgShow/msgshowwin.h"
#include "../t9input/t9widget.h"
#include "../mainmenu/mainmenu.h"
#include "options.h"



namespace Ui {
    class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = 0);
    ~LoginForm();

private:
    Ui::LoginForm *ui;
    T9Widget *input;        //���뷨���
    MainMenu *mainmenu;     //���˵����
    MsgShowWin *msgshow;    //������ʾ����
    QTimer *timer;          //��ʱ��

signals:
    void bg_show();         //ͼƬ�ֲ���������

public slots:
    void Edit_Clear();      //����ʺ����������

private slots:
    void on_checkBox_showPwd_clicked(bool checked);     //��ʾ����
    void on_Btn_cancel_clicked();                       //�˳���ǰ����
    void on_Btn_register_clicked();                     //ע���ʺ�
    void on_Btn_login_clicked();                        //������¼
    void EditText(QString);                             //�༭ѡ�п�
    void backSpaceText();                               //�˸���


protected:
    void paintEvent(QPaintEvent *);//���Ʊ���ͼ
    void mousePressEvent(QMouseEvent *);//������¼�

};

#endif // LOGINFORM_H
