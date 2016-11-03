#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "t9widget.h"
#include "form.h"
#include "mythread.h"
#include <QCheckBox>
#include <QPainter>
#include <QPaintEvent>

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
    T9Widget *input;
    Form *form;
    MyThread *thread;

private slots:
    /*
    void on_pushButton_3_clicked();
    void on_checkBox_clicked(bool checked);
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void setText(QString);
    void backSpaceText();
    */

protected:
    void paintEvent(QPaintEvent *);
};

#endif // WIDGET_H
