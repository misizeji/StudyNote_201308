#ifndef STARTMOVIE_H
#define STARTMOVIE_H

#include <QWidget>
#include <phonon/VideoPlayer>
#include <QLabel>
#include <QKeyEvent>
#include <QMouseEvent>

namespace Ui {
    class StartMovie;
}

class StartMovie : public QWidget
{
    Q_OBJECT

public:
    explicit StartMovie(QWidget *parent = 0);
    ~StartMovie();
signals:
    void movieStop();
private:
    Ui::StartMovie *ui;
    Phonon::VideoPlayer *start;
private slots:
    void doFinish();
    void doPlayMovie(int, int, int, int);
protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
};

#endif // STARTMOVIE_H
