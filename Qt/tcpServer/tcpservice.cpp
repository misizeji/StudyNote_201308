#include "netserver/tcpservice.h"


TcpService::TcpService(QObject *parent) :
    QThread(parent)
{
    this->tcpServer = new QTcpServer(this); //创建tcp服务器
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
    this->tcpServer->listen(QHostAddress::Any,port); //绑定与监听
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnect()));
        //等待连接
}

/******当有新连接时创建新的socket**************/
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

/*******接收到TCP报文时****************/
void TcpService::tcpread_slot()
{

    QByteArray msg;
    msg = tcpSocketFd->readAll();
    qDebug()<<"tcpread_slot  recieved tcp msg  "<<msg;
    emit this->haveTdpPackage(TcpMsg,//消息类型为TCpMsg
                              this->tcpSocketFd->peerAddress().toString(), //客户端ip
                              msg.data()); //接收数据，发送处理
}
