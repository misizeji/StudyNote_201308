#include "widget.h"
#include "ui_widget.h"

#include <QTime>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
	
	//设置数字按钮失效
    ui->pushButton_0->setEnabled(false);
    ui->pushButton_1->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_7->setEnabled(false);
    ui->pushButton_8->setEnabled(false);
    ui->pushButton_9->setEnabled(false);
	
	//数字按钮点击事件（只需要一个函数就行）
    connect(ui->pushButton_0,SIGNAL(clicked()),this,SLOT(Buttons_clicked()));
    connect(ui->pushButton_1,SIGNAL(clicked()),this,SLOT(Buttons_clicked()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(Buttons_clicked()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(Buttons_clicked()));
    connect(ui->pushButton_4,SIGNAL(clicked()),this,SLOT(Buttons_clicked()));
    connect(ui->pushButton_5,SIGNAL(clicked()),this,SLOT(Buttons_clicked()));
    connect(ui->pushButton_6,SIGNAL(clicked()),this,SLOT(Buttons_clicked()));
    connect(ui->pushButton_7,SIGNAL(clicked()),this,SLOT(Buttons_clicked()));
    connect(ui->pushButton_8,SIGNAL(clicked()),this,SLOT(Buttons_clicked()));
    connect(ui->pushButton_9,SIGNAL(clicked()),this,SLOT(Buttons_clicked()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_11_clicked()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int t = qrand();
    this->num = t%10000;
    qDebug()<<"num = "<<num;
    ui->pushButton_0->setEnabled(true);
    ui->pushButton_1->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_4->setEnabled(true);
    ui->pushButton_5->setEnabled(true);
    ui->pushButton_6->setEnabled(true);
    ui->pushButton_7->setEnabled(true);
    ui->pushButton_8->setEnabled(true);
    ui->pushButton_9->setEnabled(true);
}

void Widget::on_pushButton_12_clicked()
{
    this->close();
}

void Widget::Buttons_clicked()
{
    static int guessnum = 0;
    QObject *s = this->sender();
    if(s != NULL)
    {
        qDebug()<<s->objectName();
        QPushButton *button = (QPushButton *)s;
        QString str = button->text();
        if(QString::number(guessnum).size() == 4)
        {
            guessnum = 0;
        }
        guessnum = guessnum*10 + str.toInt();
        qDebug()<<"str = "<<str
                    <<", guessnum = "<<guessnum;
        if(QString::number(guessnum).size() == 4 && this->num == guessnum)
        {
            ui->textEdit->setText("you win!!");
            ui->pushButton_0->setEnabled(false);
            ui->pushButton_1->setEnabled(false);
            ui->pushButton_2->setEnabled(false);
            ui->pushButton_3->setEnabled(false);
            ui->pushButton_4->setEnabled(false);
            ui->pushButton_5->setEnabled(false);
            ui->pushButton_6->setEnabled(false);
            ui->pushButton_7->setEnabled(false);
            ui->pushButton_8->setEnabled(false);
            ui->pushButton_9->setEnabled(false);
        }
        else if(QString::number(guessnum).size() == 4 && this->num > guessnum)
        {
            ui->textEdit->setText("it is small!");
        }
        else if (QString::number(guessnum).size() == 4 && this->num > guessnum)
        {
            ui->textEdit->setText("it is big!");
        }
        ui->lcdNumber->display(guessnum);
    }
}
