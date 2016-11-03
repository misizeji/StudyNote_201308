#ifndef MOVIEWIN_H
#define MOVIEWIN_H

#include <QWidget>
#include <QProcess>
#include <unistd.h>
#include <QString>
#include <QStringList>
#include <QPainter>
#include <QDebug>
#include <QFileDialog>


namespace Ui {
    class moviewin;
}

class moviewin : public QWidget
{
    Q_OBJECT

public:
    explicit moviewin(QWidget *parent = 0);
    ~moviewin();

private:
    Ui::moviewin *ui;
    QProcess *process;      //添加linux系统命令进程
    QString fileName;

    bool isPlay;
    bool isSound;
    bool isStop;

signals:
    void show_main_menu();

private slots:
    void on_Bt_return_clicked();
    void on_HS_volume_valueChanged(int value);
    void on_Bt_forward_clicked();
    void on_Bt_PlayPause_clicked();
    void on_Bt_backward_clicked();
    void on_Bt_stop_clicked();
    void on_Bt_open_clicked();
};

#endif // MOVIEWIN_H
