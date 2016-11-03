#include "tcp_talk.h"
#include "ui_tcp_talk.h"

#include <QTimer>
#include <QDebug>
#include <QtNetwork>
#include <QString>

tcp_talk::tcp_talk(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tcp_talk)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket(this);//客户端
        tcpServer = new QTcpServer(this);//服务器

        connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
        connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
                 this,SLOT(displayError(QAbstractSocket::SocketError)));

        if(!tcpServer->listen(QHostAddress::Any,8000))
        {
             //监听本地主机的8000端口，如果出错就输出错误信息，并关闭
            qDebug() << tcpServer->errorString();
            close();
        }

        connect(tcpServer,SIGNAL(newConnection()),this,SLOT(send_report()));
        //连接信号和相应槽函数
        this->setWindowTitle("mytalk");
        this->clientConnection = new QTcpSocket(this);
}

tcp_talk::~tcp_talk()
{
    delete ui;
}

/*客户端*/
void tcp_talk::newConnect()
{
    blockSize = 0; //初始化其为0
    tcpSocket->abort(); //取消已有的连接
    tcpSocket->connectToHost(QHostAddress(ui->lineEdit_ip->text()),
                             ui->lineEdit_port->text().toInt());
    //连接到主机，这里从界面获取主机地址和端口号
}

void tcp_talk::readMessage()
{
    QByteArray msg;
    msg = tcpSocket->readAll();
    qDebug()<<"tcpread_slot  recieved tcp msg  "<<msg;
//    QDataStream in(tcpSocket);
//    in.setVersion(QDataStream::Qt_4_6);
//    //设置数据流版本，这里要和服务器端相同
//    if(blockSize==0) //如果是刚开始接收数据
//    {
//        //判断接收的数据是否有两字节，也就是文件的大小信息
//        //如果有则保存到blockSize变量中，没有则返回，继续接收数据
//        if(tcpSocket->bytesAvailable() < (int)sizeof(quint16)) return;
//        in >> blockSize;
//    }

//    if(tcpSocket->bytesAvailable() < blockSize) return;
//    //如果没有得到全部的数据，则返回，继续接收数据
//    in >> message;

//    qDebug()<<"message==================="<<message;
//    //将接收到的数据存放到变量中
//    QString fromIP = QString("%1:").arg(ui->lineEdit_ip->text());
//    ui->textBrowser_talk_win->setText(fromIP);//对方的IP
//    ui->textBrowser_talk_win->append(message);
//    //显示接收到的数据
}

void tcp_talk::displayError(QAbstractSocket::SocketError)
{
    qDebug() << tcpSocket->errorString(); //输出错误信息
}

void tcp_talk::on_toolButton_connect_clicked()//建立链接
{
    newConnect(); //请求连接
}

/*服务器*/
void tcp_talk::send_report()
{
    QByteArray block; //用于暂存我们要发送的数据
    QDataStream out(&block,QIODevice::WriteOnly);
    //使用数据流写入数据
    out.setVersion(QDataStream::Qt_4_6);
    //设置数据流的版本，客户端和服务器端使用的版本要相同
    out<<(quint16) 0;
    out<<tr("link successful!!!");
    out.device()->seek(0);
    out<<(quint16) (block.size()-sizeof(quint16));
    clientConnection = tcpServer->nextPendingConnection();

    qDebug()<<"__000_____"<<clientConnection;
    //我们获取已经建立的连接的子套接字
    //connect(clientConnection,SIGNAL(disconnected()),clientConnection,SLOT(deleteLater()));
    clientConnection->write(block);
    //qDebug()<<clientConnection->isOpen();
    //clientConnection->disconnectFromHost();
}

void tcp_talk::on_pushButton_send_clicked()//发送信息
{
    QByteArray send_buf; //用于暂存我们要发送的数据
    QDataStream out(&send_buf,QIODevice::WriteOnly);
    //使用数据流写入数据
    out.setVersion(QDataStream::Qt_4_6);
    //设置数据流的版本，客户端和服务器端使用的版本要相同
    out<<(quint16) 0;
    QString send_msg = ui->textEdit_write_send->toPlainText();
    //out<<send_msg;
    out<<tr("hello world!!!");
    out.device()->seek(0);
    out<<(quint16) (send_buf.size()-sizeof(quint16));

    qDebug()<<"__001_____"<<clientConnection;
    //qDebug()<<clientConnection->isOpen();
    qDebug()<<"send size = "<<clientConnection->write(send_buf);
    ui->textEdit_write_send->setText("");//清空信息输入框
}
