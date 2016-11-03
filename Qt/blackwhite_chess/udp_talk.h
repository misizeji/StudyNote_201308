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
    QUdpSocket *sender;//�ͻ���
    QUdpSocket *receiver;//������

private slots:
    void on_pushButton_send_clicked();//���͹㲥
    void processPendingDatagram();//��������������


};

#endif // UDP_TALK_H
