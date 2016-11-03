#include "netserver/tcpservice.h"


TcpService::TcpService(QObject *parent) :
    QThread(parent)
{
    this->tcpServer = new QTcpServer(this); //����tcp������
    tcpSocketFd=NULL;
}
void TcpService::tcpFdreadyrd()
{
 if(this->tcpSocketFd==NULL)
     return;
 connect(this->tcpSocketFd, SIGNAL(readyRead()),this,SLOT(tcpread_slot()));
}
void TcpService::setMyip(QString ip)
{
    myip=ip;
}

void TcpService::sendTcpMsg(QString msg)
{
    if(tcpSocketFd != NULL)
        this->tcpSocketFd->write(msg.toAscii());
    //qDebug()<<"in tcpservice sendTcpMsg"<<msg;
}

void TcpService::setport(int port)
{
    this->port=port;
    this->tcpServer->listen(QHostAddress::Any,port); //�������
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnect()));
        //�ȴ�����
}

/******����������ʱ�����µ�socket**************/
void TcpService::acceptConnect()
{
    this->tcpSocketFd = this->tcpServer->nextPendingConnection();
        QString ipcome = tcpSocketFd->peerAddress().toString();
        if(ipcome==myip)
            return;
        qDebug()<<"newConnection "<<ipcome;
        QString msg=QString("1:1:1:1:%1:").arg(IPMSG_ANSTCPCONNECT);
        tcpSocketFd->write(msg.toAscii());
        qDebug()<<"tcp write anstcpconnect after";
        connect(this->tcpSocketFd, SIGNAL(readyRead()),this,SLOT(tcpread_slot()));
}

/*******���յ�TCP����ʱ****************/
void TcpService::tcpread_slot()
{

    QByteArray msg;
    msg = tcpSocketFd->readAll();
    qDebug()<<"tcpread_slot  recieved tcp msg  "<<msg;
    emit this->haveTdpPackage(TcpMsg,//��Ϣ����ΪTCpMsg
                              this->tcpSocketFd->peerAddress().toString(), //�ͻ���ip
                              msg.data()); //�������ݣ����ʹ���
}
