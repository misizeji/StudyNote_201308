#ifndef MYLINK_H
#define MYLINK_H

#include <QWidget>

namespace Ui {
    class mylink;
}

class mylink : public QWidget
{
    Q_OBJECT

public:
    explicit mylink(QWidget *parent = 0);
    ~mylink();
    QString get_ip();
    QString get_port();
signals:
    void get_online();
private:
    Ui::mylink *ui;
    QString ip;
    QString port;

private slots:
    void on_pushButton_link_clicked();
};

#endif // MYLINK_H
