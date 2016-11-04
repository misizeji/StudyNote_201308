#include "udp_talk.h"
#include "ui_udp_talk.h"

#include <QString>
#include <QStringList>

udp_talk::udp_talk(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::udp_talk)
{
    ui->setupUi(this);

    sender = new QUdpSocket(this);//�ͻ��˽���udp�����׽���
    receiver = new QUdpSocket(this);//����������udp�����׽���
    receiver->bind(QHostAddress::Any,8000,QUdpSocket::ShareAddress);//��8000�˿�

    connect(receiver,SIGNAL(readyRead()),this,SLOT(processPendingDatagram()));//����źŽ������ݶ�ȡ
}

udp_talk::~udp_talk()
{
    delete ui;
}

void udp_talk::on_pushButton_send_clicked()//���ͺ���
{
    QByteArray datagram = ui->textEdit_write_send->toPlainText().toAscii();
        sender->writeDatagram(datagram.data(),datagram.size(),
                              QHostAddress(ui->lineEdit_ip->text()),ui->lineEdit_port->text().toInt());
        ui->textEdit_write_send->setText("");//��������
}

void udp_talk::processPendingDatagram() //����ȴ������ݱ�
{
    QString fromIP = QString("%1:").arg(ui->lineEdit_ip->text());
    ui->textBrowser_talk_win->setText(fromIP);//�Է���IP

    QByteArray datagram; //���ڴ�Ž��յ����ݱ�
    while(receiver->hasPendingDatagrams())  //ӵ�еȴ������ݱ�
    {
        datagram.resize(receiver->pendingDatagramSize());
        //��datagram�Ĵ�СΪ�ȴ���������ݱ��Ĵ�С���������ܽ��յ�����������
        receiver->readDatagram(datagram.data(),datagram.size());
        //�������ݱ��������ŵ�datagram��
    }
    //�����ݱ�������ʾ����
    ui->textBrowser_talk_win->append(datagram);
}

