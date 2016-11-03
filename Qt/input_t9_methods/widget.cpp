#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

//C++�в���C���Եķ�ʽ
extern "C"  //�Ǵ�д�� C
{
    #include"./set_led/led.h"
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    input = new T9Widget(ui->lineEdit,this);
    input->move(100,100);
    input->show();

    connect(input,SIGNAL(newString(QString)),
            this,SLOT(setText(QString)));

    connect(input,SIGNAL(backSpace()),
            this,SLOT(backSpaceText()));

}

Widget::~Widget()
{
    delete ui;
}

void Widget::setText(QString str)
{
    if(ui->lineEdit->hasFocus() == true )   //�жϽ�������
    {
        qDebug()<<"--------";
        ui->lineEdit->insert(str);  //׷��
    }
    else if(ui->lineEdit_2->hasFocus()==true)
    {
        qDebug()<<"+++++++++";
        ui->lineEdit_2->insert(str);
    }
}

void Widget::backSpaceText()    //�����˸��
{
    QString str;
    if(ui->lineEdit->hasFocus() == true )
    {
        str = ui->lineEdit->text(); //���Ȼ�õ��ı����е�����
        if(str.isEmpty() == false)
        {
            str = str.left(str.size()-1);   //�ַ���һ
            ui->lineEdit->setText(str); //��������
        }
    }
    else if(ui->lineEdit_2->hasFocus() == true)
    {
        str = ui->lineEdit_2->text();
        if(str.isEmpty() == false)
        {
            str = str.left(str.size()-1);
            ui->lineEdit_2->setText(str);
        }
    }

}
