#ifndef USERMNGWIN_H
#define USERMNGWIN_H

#include <QWidget>
#include <QDialog>
#include <QtGui>
#include <QtSql>
#include "../t9input/t9widget.h"

namespace Ui {
    class UserMngWin;
}

class UserMngWin : public QWidget
{
    Q_OBJECT

public:
    explicit UserMngWin(QWidget *parent = 0);
    ~UserMngWin();

private:
    Ui::UserMngWin *ui;
    QSqlTableModel *model;
    T9Widget *input;        // ‰»Î∑®ÃÌº”
    bool showflag;

signals:
    void show_main_menu();

private slots:
    void on_quitButton_clicked();
    void inputshow();
    void submit();


};

#endif // USERMNGWIN_H
