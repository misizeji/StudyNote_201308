#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QSound>
#include <QTimer>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = 0);


public slots:
    void play_music();
signals:
    void music_play();
protected:
    void run();
private:
    QSound music;
    int ret_key;
};

#endif // MYTHREAD_H
