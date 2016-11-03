#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

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
    int num;

private slots:
    void Buttons_clicked();
    void on_pushButton_12_clicked();
    void on_pushButton_11_clicked();
};

#endif // WIDGET_H
