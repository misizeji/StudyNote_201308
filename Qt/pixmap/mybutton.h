#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QWidget>
#include <QPushButton>

class MyButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyButton(QWidget *parent = 0);

signals:

public slots:
    void mouseMoveEvent(QMouseEvent *e);

};

#endif // MYBUTTON_H
