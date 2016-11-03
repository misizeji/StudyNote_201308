#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = 0);
    void setNum(int);
signals:

public slots:

protected:
    void run();
private:
    int num;
};

#endif // MYTHREAD_H
