#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>
#include <QMessageBox>
#include <QHostInfo>
#include <QNetworkInterface>
#include "t9input/t9widget.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QHostAddress *localHostAddr;
    QHostAddress *remoteHostAddr;
    QString localIpStr;
    QString remoteIpStr;
    QString getIp();
    void autoScroll();

private slots:

    void on_T9inputButton_clicked();
    void on_pingButtion_clicked();
    void on_configButtion_clicked();

    void send();
    void receive();

    int write_msg_in_temp(const char *s);
    int read_msg_in_temp(const QString& fpath);

    void on_clearButton_clicked();
    void on_configButton_clicked();
    void on_exitButton_clicked();

    void setText(QString);
    void backSpaceText();

private:
    Ui::Widget *ui;
    T9Widget *input;
    QUdpSocket *udpSocket1;   
    bool configFlag;
};

#endif // WIDGET_H
