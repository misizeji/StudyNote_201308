#ifndef UDP_TALK_H
#define UDP_TALK_H

#include <QWidget>
#include <QtNetwork>

namespace Ui {
    class udp_talk;
}

class udp_talk : public QWidget
{
    Q_OBJECT

public:
    explicit udp_talk(QWidget *parent = 0);
    ~udp_talk();

private:
    Ui::udp_talk *ui;
    QUdpSocket *sender;//客户端
    QUdpSocket *receiver;//服务器

private slots:
    void on_pushButton_send_clicked();//发送广播
    void processPendingDatagram();//服务器接收数据


};

#endif // UDP_TALK_H
