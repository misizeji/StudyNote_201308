#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

//C++中插入C语言的方式
extern "C"  //是大写的 C
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
    if(ui->lineEdit->hasFocus() == true )   //判断焦点在哪
    {
        qDebug()<<"--------";
        ui->lineEdit->insert(str);  //追加
    }
    else if(ui->lineEdit_2->hasFocus()==true)
    {
        qDebug()<<"+++++++++";
        ui->lineEdit_2->insert(str);
    }
}

void Widget::backSpaceText()    //按了退格键
{
    QString str;
    if(ui->lineEdit->hasFocus() == true )
    {
        str = ui->lineEdit->text(); //首先获得到文本框中的内容
        if(str.isEmpty() == false)
        {
            str = str.left(str.size()-1);   //字符减一
            ui->lineEdit->setText(str); //重新设置
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
