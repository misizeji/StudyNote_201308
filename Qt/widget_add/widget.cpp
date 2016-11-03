#include "widget.h"
#include "ui_widget.h"

#include <QPalette>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    la = new QLabel(this);//建立新的标签
    QPalette p;			//新建背景方案
    p.setColor(QPalette::Window,Qt::magenta);//设置粉色

    la->setPalette(p);
    la->setAutoFillBackground(true);//自动填充

//    ui->gridLayout->addWidget(la,0,0,1,1);
    ui->verticalLayout_2->addWidget(la);//将啦窗口添加到ui的窗口布局
}

Widget::~Widget()
{
    delete ui;
}
