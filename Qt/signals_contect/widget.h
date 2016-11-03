#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include "myform.h"

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
    QTimer *timer;
    int t;
    int num;
    MyForm *form1;
    MyForm *form2;
public slots:
    void myslot();
    void buttons_clicked();
    void setText(QString);
private slots:
    void on_pushButton_clicked();
    void on_pushButtonTimer_clicked();
    void on_comboBox_activated(QString str);
};

#endif // WIDGET_H
