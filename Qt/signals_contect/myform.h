#ifndef MYFORM_H
#define MYFORM_H

#include <QWidget>

namespace Ui {
    class MyForm;
}

class MyForm : public QWidget
{
    Q_OBJECT

public:
    explicit MyForm(QWidget *parent = 0);
    ~MyForm();
    void setbutton(bool);

private:
    Ui::MyForm *ui;

signals:
    void mySignal();
    void newText(QString);
private slots:
    void on_textEdit_textChanged();
    void on_pushButton_clicked();
};

#endif // MYFORM_H
