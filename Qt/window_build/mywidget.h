#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QtGui/QWidget>

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    MyWidget(QWidget *parent = 0);
    ~MyWidget();
};

#endif // MYWIDGET_H
