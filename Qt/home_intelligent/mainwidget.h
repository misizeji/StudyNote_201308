#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

#include "./loginform/loginform.h"
#include "options.h"

namespace Ui {
    class mainWidget;
}

class mainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit mainWidget(QWidget *parent = 0);
    ~mainWidget();

private:
    Ui::mainWidget *ui;
    QTimer *timer;              //��ʱ��
    QString pic_bg;             //����ͼƬ����
    QStringList pic_bg_list;    //�����ַ���ͼƬ����
    LoginForm *login_form;      //��¼����

private slots:
    void pic_bg_show();     //����ͼƬ�Զ��л�����
    void widget_show();     //����ͼƬ�����ֲ�



protected:
    void paintEvent(QPaintEvent *);//���Ʊ���ͼ
    void mousePressEvent(QMouseEvent *);//������¼�


};

#endif // MAINWIDGET_H
