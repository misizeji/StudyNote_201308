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
    QTcpSocket *tcpSocket;  //�ͻ���
    QTcpServer *tcpServer;  //������
    QTcpSocket *clientConnection;//���������������ӵ����׽���
    QString message;  //��Ŵӷ��������յ����ַ���
    quint16 blockSize;  //����ļ��Ĵ�С��Ϣ
private slots:
    void on_pushButton_send_clicked();//���Ͱ�ť
    void on_toolButton_connect_clicked();//��������
    void newConnect(); //���ӷ�����
    void readMessage();  //��������
    void displayError(QAbstractSocket::SocketError);//��ʾ����
    void send_report();//���ӱ���

};

#endif // TCP_TALK_H
