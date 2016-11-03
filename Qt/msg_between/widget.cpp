#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_verticalSlider_valueChanged(int value)//滑动条发生值变化
{
    ui->lcdNumber->display(value);//lcd显示
    ui->dial->setValue(value);;
}

void Widget::on_dial_sliderMoved(int position)//旋钮发生变化
{
    ui->lcdNumber->display(position);
    ui->verticalSlider->setValue(position);
}
