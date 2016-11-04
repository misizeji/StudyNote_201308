#include "udp_talk.h"
#include "ui_udp_talk.h"

#include <QString>
#include <QStringList>

udp_talk::udp_talk(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::udp_talk)
{
    ui->setupUi(this);

    sender = new QUdpSocket(this);//客户端建立udp发送套接字
    receiver = new QUdpSocket(this);//服务器建立udp接收套接字
    receiver->bind(QHostAddress::Any,8000,QUdpSocket::ShareAddress);//绑定8000端口

    connect(receiver,SIGNAL(readyRead()),this,SLOT(processPendingDatagram()));//检测信号进入数据读取
}

udp_talk::~udp_talk()
{
    delete ui;
}

void udp_talk::on_pushButton_send_clicked()//发送函数
{
    QByteArray datagram = ui->textEdit_write_send->toPlainText().toAscii();
        sender->writeDatagram(datagram.data(),datagram.size(),
                              QHostAddress(ui->lineEdit_ip->text()),ui->lineEdit_port->text().toInt());
        ui->textEdit_write_send->setText("");//清空输入框
}

void udp_talk::processPendingDatagram() //处理等待的数据报
{
    QString fromIP = QString("%1:").arg(ui->lineEdit_ip->text());
    ui->textBrowser_talk_win->setText(fromIP);//对方的IP

    QByteArray datagram; //用于存放接收的数据报
    while(receiver->hasPendingDatagrams())  //拥有等待的数据报
    {
        datagram.resize(receiver->pendingDatagramSize());
        //让datagram的大小为等待处理的数据报的大小，这样才能接收到完整的数据
        receiver->readDatagram(datagram.data(),datagram.size());
        //接收数据报，将其存放到datagram中
    }
    //将数据报内容显示出来
    ui->textBrowser_talk_win->append(datagram);
}

