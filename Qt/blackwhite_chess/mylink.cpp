#include "mylink.h"
#include "ui_mylink.h"

#include <QString>

mylink::mylink(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mylink)
{
    ui->setupUi(this);
}

mylink::~mylink()
{
    delete ui;
}

void mylink::on_pushButton_link_clicked()//获取数据
{
    this->ip = ui->lineEdit_ip->text();
    this->port = ui->lineEdit_port->text();
    emit get_online();  //发送上线
    ui->pushButton_link->setEnabled(false);
    this->hide();//隐藏窗口
}

QString mylink::get_ip()
{
    return this->ip;
}

QString mylink::get_port()
{
    return this->port;
}
