#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#define EASY   10
#define NORMAL 11
#define HARD   12
#define PVP    13

#define SHOW   0
#define HIDE   1

#define SIZE_ON  0
#define SIZE_OFF 1

#define SIZE_UP     0
#define SIZE_DONW   1
#define SIZE_LEFT   2
#define SIZE_RIGHT  3
#define SIZE_L_U    4
#define SIZE_L_D    5
#define SIZE_R_U    6
#define SIZE_R_D    7


#include <QWidget>
#include <QSound>
#include <QMouseEvent>


namespace Ui {
    class StartWindow;
}

class StartWindow : public QWidget
{
    Q_OBJECT

public:
    QTimer *windowTime;
    explicit StartWindow(QWidget *parent = 0);
    ~StartWindow();

private:
    Ui::StartWindow *ui;
    int w;
    int h;
    int mode;
    int flag;
    int sizeFlag;
    int sizeChange;
    int maxflag;
    int moveflag;
    int moveW;
    int moveH;
    QSound *song;
    QTimer *ti;

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
signals:
    void begin(int);
    void playMovie(int, int, int, int);
private slots:
    void on_movieToolButton_clicked();
    void on_quitToolButton_clicked();
    void on_maxToolButton_clicked();
    void on_minToolButton_clicked();
    void doTimesUp();
    void doWindowTime();
    void doMovieStop();
    void on_pvpToolButton_clicked();
    void on_hardToolButton_clicked();
    void on_normalToolButton_clicked();
    void on_easyToolButton_clicked();
    void on_cancelToolButton_clicked();
    void on_pvcToolButton_clicked();
    void on_extiToolButton_clicked();
    void on_backToolButton2_clicked();
    void on_backToolButton1_clicked();
    void on_aboutToolButton_clicked();
    void on_startToolButton_clicked();
};

#endif // STARTWINDOW_H
