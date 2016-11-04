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

void mylink::on_pushButton_link_clicked()//��ȡ����
{
    this->ip = ui->lineEdit_ip->text();
    this->port = ui->lineEdit_port->text();
    emit get_online();  //��������
    ui->pushButton_link->setEnabled(false);
    this->hide();//���ش���
}

QString mylink::get_ip()
{
    return this->ip;
}

QString mylink::get_port()
{
    return this->port;
}
