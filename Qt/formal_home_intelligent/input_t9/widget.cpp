#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QSound>

//C++中插入C语言的方式
extern "C"  //是大写的 C
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
    /******************添加键盘***********************/
    input = new T9Widget(ui->lineEdit,this);
    input->move(450,90);
    input->show();

    /*****************键盘联系**********************/
    connect(input,SIGNAL(newString(QString)),this,SLOT(setText(QString)));
    connect(input,SIGNAL(backSpace()),this,SLOT(backSpaceText()));

    setAttribute(Qt::WA_TranslucentBackground,true);
    setWindowFlags(Qt::FramelessWindowHint);
    this->showFullScreen();

    /***************创建线程*****************/
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
    if(ui->lineEdit->hasFocus() == true )   //判断焦点在哪
    {
        ui->lineEdit->insert(str);  //追加
    }
    else if(ui->lineEdit_2->hasFocus()==true)
    {
        ui->lineEdit_2->insert(str);
    }
}
*/

/*

void Widget::backSpaceText()    //按了退格键
{
    QString str;
    if(ui->lineEdit->hasFocus() == true )
    {
        str = ui->lineEdit->text(); //首先获得到文本框中的内容
        if(str.isEmpty() == false)
        {
            str = str.left(str.size()-1);   //字符减一
            ui->lineEdit->setText(str); //重新设置
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
void Widget::on_pushButton_clicked()//正常登陆
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

/*********************退出*************************************/
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

/*******************贴背景图片**********************/
void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,800,480,QPixmap(":/new/prefix2/9.png"));
}

/*
void Widget::on_pushButton_3_clicked()//注册账号成功
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
