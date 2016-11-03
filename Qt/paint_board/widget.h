#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QColor>
#include "scribblearea.h"

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
    ScribbleArea *area;
    QColor *mycolor;

private slots:
    void on_toolButton_clicked();
    void on_spinBox_valueChanged(int );
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void mouseMoveEvent(QMouseEvent *);
};

#endif // WIDGET_H
