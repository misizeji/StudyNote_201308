#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //connect(ui->pushButtonHide,SIGNAL(clicked()),ui->label,SLOT(hide()));
    //connect(ui->comboBox,SIGNAL(currentIndexChanged(QString)),ui->label_2,
           //  SLOT(setText(QString)));
    this->timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(myslot()));//计时器定时信号连接
    t = 30;
    ui->lcdNumber->display(t);

    num = 0;
    connect(ui->pushButton_1,SIGNAL(clicked()),this,SLOT(buttons_clicked()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(buttons_clicked()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(buttons_clicked()));
    connect(ui->pushButton_4,SIGNAL(clicked()),this,SLOT(buttons_clicked()));

    this->form1 = new MyForm;
    connect(form1,SIGNAL(mySignal()),this,SLOT(show()));
    this->form2 = new MyForm;
    ui->horizontalLayout_2->addWidget(form2);
    form2->setbutton(false);
    connect(form2,SIGNAL(newText(QString)),this,SLOT(setText(QString)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_comboBox_activated(QString str)
{
    qDebug()<<"________str is _________"<<str;
    ui->label_2->setText(str);
}

void Widget::myslot()//自定义信号处理函数
{
    t--;
    if(t == -1)
        t = 30;
    ui->lcdNumber->display(t);//lcd倒计时
}

void Widget::on_pushButtonTimer_clicked()//倒计时按钮事件
{
    if(timer->isActive() == true)
    {
        timer->stop();
        ui->pushButtonTimer->setText("start");
    }
    else
    {
        timer->start(1000);
        ui->pushButtonTimer->setText("stop");
    }
}
void Widget::buttons_clicked()
{
    QObject *s = this->sender();//获取信号发送者
    if(s != NULL)
    {
        qDebug()<<s->objectName();//得到信号发送者名称
        QPushButton *button = (QPushButton *)s;
        QString str = button->text();
        num = num*10 + str.toInt();
        if(num < 0)
        {
            num = 0;
        }
        qDebug()<<"str = "<<str
                <<", num = "<<num;
        ui->lcdNumber_2->setDigitCount(QString::number(num).size());
        ui->lcdNumber_2->display(num);
    }
}

void Widget::on_pushButton_clicked()
{
    form1->setGeometry(this->geometry());
    this->form1->show();
    this->hide();
}

void Widget::setText(QString str)
{
    ui->textEdit->setText("");
    ui->textEdit->append(str);
}
