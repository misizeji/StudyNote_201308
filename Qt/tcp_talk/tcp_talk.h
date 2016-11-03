#ifndef TCP_TALK_H
#define TCP_TALK_H

#include <QWidget>
#include <QWidget>
#include <QtNetwork>
#include <QTcpSocket>

namespace Ui {
    class tcp_talk;
}

class tcp_talk : public QWidget
{
    Q_OBJECT

public:
    explicit tcp_talk(QWidget *parent = 0);
    ~tcp_talk();

private:
    Ui::tcp_talk *ui;
    QTcpSocket *tcpSocket;  //客户端
    QTcpServer *tcpServer;  //服务器
    QTcpSocket *clientConnection;//服务器建立的连接的子套接字
    QString message;  //存放从服务器接收到的字符串
    quint16 blockSize;  //存放文件的大小信息
private slots:
    void on_pushButton_send_clicked();//发送按钮
    void on_toolButton_connect_clicked();//建立链接
    void newConnect(); //连接服务器
    void readMessage();  //接收数据
    void displayError(QAbstractSocket::SocketError);//显示错误
    void send_report();//链接报告

};

#endif // TCP_TALK_H
