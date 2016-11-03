#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "mybutton.h"

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    //MyButton *button;
    int startX;
    int x,y;
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

private slots:
    void on_pushButton_clicked();
};

#endif // WIDGET_H
