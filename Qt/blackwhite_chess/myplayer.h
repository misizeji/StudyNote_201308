#ifndef MYPLAYER_H
#define MYPLAYER_H

#include <QWidget>
#include <phonon/VideoPlayer>
#include <QProcess>

namespace Ui {
    class MyPlayer;
}

class MyPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit MyPlayer(QWidget *parent = 0);
    void openFile(const QString &fileName);
    ~MyPlayer();
signals:

public slots:
    void open();
    void goToFrame(int frame);
    void updateFrameSlider();
    void mypause();
    void mystop();
    void myplay();
    void myclose();

private:
    Ui::MyPlayer *ui;
    QString fileName;
    QString currentMovieDirectory;
    Phonon::VideoPlayer *player;
    QTimer *timer;
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MYPLAYER_H
