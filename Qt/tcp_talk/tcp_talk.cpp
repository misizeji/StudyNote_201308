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
    tcpSocket = new QTcpSocket(this);//�ͻ���
        tcpServer = new QTcpServer(this);//������

        connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
        connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
                 this,SLOT(displayError(QAbstractSocket::SocketError)));

        if(!tcpServer->listen(QHostAddress::Any,8000))
        {
             //��������������8000�˿ڣ������������������Ϣ�����ر�
            qDebug() << tcpServer->errorString();
            close();
        }

        connect(tcpServer,SIGNAL(newConnection()),this,SLOT(send_report()));
        //�����źź���Ӧ�ۺ���
        this->setWindowTitle("mytalk");
        this->clientConnection = new QTcpSocket(this);
}

tcp_talk::~tcp_talk()
{
    delete ui;
}

/*�ͻ���*/
void tcp_talk::newConnect()
{
    blockSize = 0; //��ʼ����Ϊ0
    tcpSocket->abort(); //ȡ�����е�����
    tcpSocket->connectToHost(QHostAddress(ui->lineEdit_ip->text()),
                             ui->lineEdit_port->text().toInt());
    //���ӵ�����������ӽ����ȡ������ַ�Ͷ˿ں�
}

void tcp_talk::readMessage()
{
    QByteArray msg;
    msg = tcpSocket->readAll();
    qDebug()<<"tcpread_slot  recieved tcp msg  "<<msg;
//    QDataStream in(tcpSocket);
//    in.setVersion(QDataStream::Qt_4_6);
//    //�����������汾������Ҫ�ͷ���������ͬ
//    if(blockSize==0) //����Ǹտ�ʼ��������
//    {
//        //�жϽ��յ������Ƿ������ֽڣ�Ҳ�����ļ��Ĵ�С��Ϣ
//        //������򱣴浽blockSize�����У�û���򷵻أ�������������
//        if(tcpSocket->bytesAvailable() < (int)sizeof(quint16)) return;
//        in >> blockSize;
//    }

//    if(tcpSocket->bytesAvailable() < blockSize) return;
//    //���û�еõ�ȫ�������ݣ��򷵻أ�������������
//    in >> message;

//    qDebug()<<"message==================="<<message;
//    //�����յ������ݴ�ŵ�������
//    QString fromIP = QString("%1:").arg(ui->lineEdit_ip->text());
//    ui->textBrowser_talk_win->setText(fromIP);//�Է���IP
//    ui->textBrowser_talk_win->append(message);
//    //��ʾ���յ�������
}

void tcp_talk::displayError(QAbstractSocket::SocketError)
{
    qDebug() << tcpSocket->errorString(); //���������Ϣ
}

void tcp_talk::on_toolButton_connect_clicked()//��������
{
    newConnect(); //��������
}

/*������*/
void tcp_talk::send_report()
{
    QByteArray block; //�����ݴ�����Ҫ���͵�����
    QDataStream out(&block,QIODevice::WriteOnly);
    //ʹ��������д������
    out.setVersion(QDataStream::Qt_4_6);
    //�����������İ汾���ͻ��˺ͷ�������ʹ�õİ汾Ҫ��ͬ
    out<<(quint16) 0;
    out<<tr("link successful!!!");
    out.device()->seek(0);
    out<<(quint16) (block.size()-sizeof(quint16));
    clientConnection = tcpServer->nextPendingConnection();

    qDebug()<<"__000_____"<<clientConnection;
    //���ǻ�ȡ�Ѿ����������ӵ����׽���
    //connect(clientConnection,SIGNAL(disconnected()),clientConnection,SLOT(deleteLater()));
    clientConnection->write(block);
    //qDebug()<<clientConnection->isOpen();
    //clientConnection->disconnectFromHost();
}

void tcp_talk::on_pushButton_send_clicked()//������Ϣ
{
    QByteArray send_buf; //�����ݴ�����Ҫ���͵�����
    QDataStream out(&send_buf,QIODevice::WriteOnly);
    //ʹ��������д������
    out.setVersion(QDataStream::Qt_4_6);
    //�����������İ汾���ͻ��˺ͷ�������ʹ�õİ汾Ҫ��ͬ
    out<<(quint16) 0;
    QString send_msg = ui->textEdit_write_send->toPlainText();
    //out<<send_msg;
    out<<tr("hello world!!!");
    out.device()->seek(0);
    out<<(quint16) (send_buf.size()-sizeof(quint16));

    qDebug()<<"__001_____"<<clientConnection;
    //qDebug()<<clientConnection->isOpen();
    qDebug()<<"send size = "<<clientConnection->write(send_buf);
    ui->textEdit_write_send->setText("");//�����Ϣ�����
}
