#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "t9input/t9widget.h"

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
private slots:
    void setText(QString);
    void backSpaceText();
};

#endif // WIDGET_H
