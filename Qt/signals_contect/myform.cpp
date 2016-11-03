#include "myform.h"
#include "ui_myform.h"

MyForm::MyForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyForm)
{
    ui->setupUi(this);
}

MyForm::~MyForm()
{
    delete ui;
}

void MyForm::on_pushButton_clicked()
{
    this->hide();
    emit this->mySignal(); //·¢ËÍÐÅºÅ
}

void MyForm::setbutton(bool f)
{
    ui->pushButton->setVisible(f);
}

void MyForm::on_textEdit_textChanged()
{
    QString str = ui->textEdit->toPlainText();
    emit this->newText(str);
}
