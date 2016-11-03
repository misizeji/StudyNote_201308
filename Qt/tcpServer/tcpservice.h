#ifndef TCPSERVICE_H
#define TCPSERVICE_H

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QThread>
#include "netserver/IPMSG.H"

class TcpService : public QThread
{
    Q_OBJECT
public:
    explicit TcpService(QObject *parent = 0);
    QTcpSocket *tcpSocketFd;
    QTcpServer *tcpServer;
    void setport(int port);
    void sendTcpMsg(QString msg);
    void setMyip(QString ip);
    void tcpFdreadyrd();
signals:
    void haveTdpPackage(ParseCmd, const QString &ip, const char *content);
public slots:
    void acceptConnect();
    void tcpread_slot();
private:
    QString myip;
    int port;
};

#endif // TCPSERVICE_H
