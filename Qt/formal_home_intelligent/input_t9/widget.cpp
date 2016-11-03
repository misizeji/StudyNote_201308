#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QSound>

//C++�в���C���Եķ�ʽ
extern "C"  //�Ǵ�д�� C
{
    /*
    #include "account_register.h"
    #include "normal_login.h"
    #include "sqlite3.h"
     */
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    form = new Form;
    form->hide();
    /******************��Ӽ���***********************/
    input = new T9Widget(ui->lineEdit,this);
    input->move(450,90);
    input->show();

    /*****************������ϵ**********************/
    connect(input,SIGNAL(newString(QString)),this,SLOT(setText(QString)));
    connect(input,SIGNAL(backSpace()),this,SLOT(backSpaceText()));

    setAttribute(Qt::WA_TranslucentBackground,true);
    setWindowFlags(Qt::FramelessWindowHint);
    this->showFullScreen();

    /***************�����߳�*****************/
    /*
    this->thread = new MyThread(this);
    this->thread->start();
    */
}

Widget::~Widget()
{
    delete ui;
}

/*
void Widget::setText(QString str)
{
    if(ui->lineEdit->hasFocus() == true )   //�жϽ�������
    {
        ui->lineEdit->insert(str);  //׷��
    }
    else if(ui->lineEdit_2->hasFocus()==true)
    {
        ui->lineEdit_2->insert(str);
    }
}
*/

/*

void Widget::backSpaceText()    //�����˸��
{
    QString str;
    if(ui->lineEdit->hasFocus() == true )
    {
        str = ui->lineEdit->text(); //���Ȼ�õ��ı����е�����
        if(str.isEmpty() == false)
        {
            str = str.left(str.size()-1);   //�ַ���һ
            ui->lineEdit->setText(str); //��������
        }
    }
    else if(ui->lineEdit_2->hasFocus() == true)
    {
        str = ui->lineEdit_2->text();
        if(str.isEmpty() == false)
        {
            str = str.left(str.size()-1);
            ui->lineEdit_2->setText(str);
        }
    }
}
*/

/*
void Widget::on_pushButton_clicked()//������½
{
    int ret = 1;//normal_login(ui->lineEdit->text().toAscii().data(),
                           //ui->lineEdit_2->text().toAscii().data());
    if(ret == 1)
    {
        this->hide();
        form->setGeometry(this->geometry());
        form->show();
    }
    else if(ret == 0)
    {
        QMessageBox::warning(this,"Warning","UserName or Password Error!");
    }
    else
    {
         qDebug()<<"program worng or db is not exits!!";
    }
}
*/

/*********************�˳�*************************************/
/*
void Widget::on_pushButton_2_clicked()
{
    this->close();
}

void Widget::on_checkBox_clicked(bool checked)
{
    if(checked)
    {
        ui->lineEdit_2->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    }
}

*/

/*******************������ͼƬ**********************/
void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,800,480,QPixmap(":/new/prefix2/9.png"));
}

/*
void Widget::on_pushButton_3_clicked()//ע���˺ųɹ�
{
    int ret = 1;//account_register(ui->lineEdit->text().toAscii().data(),
                               //ui->lineEdit_2->text().toAscii().data());
    if(ret == 1)
    {
        QMessageBox::warning(this,"congratulation","register successful!!");
    }
    else if(ret == 0)
    {
        QMessageBox::warning(this,"Warning","UserName or Password Error!");
    }
    else
    {
         qDebug()<<"program worng or db is not exits!!";
    }
}
*/
