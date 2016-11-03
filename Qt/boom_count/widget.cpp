#include "widget.h"
#include "ui_widget.h"
#include <QMovie>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->label->hide();
    this->timer = new QTimer(this);//���ö�ʱ��
    connect(timer,SIGNAL(timeout()),this,SLOT(myslot()));//���Ӷ�ʱ���ź�
    timer->start(1000);
    t = 10;
    ui->lcdNumber->display(t);//lcd��ʾ����
}

Widget::~Widget()
{
    delete ui;
}

void Widget::myslot()
{
    t--;
    if(t == 0)
    {
        ui->lcdNumber->close();	//��ʱ�����ر�lcd����
        QMovie *movie = new QMovie(this);//����movie����
        movie->setFileName(":/new/pic/boom.gif");//����gif����ͼƬ
        ui->label->setMovie(movie);//��label��ʾͼƬ
        movie->start();//������ʾ
        ui->label->setScaledContents(true);//�����Զ����
        //ui->label->setWindowFlags(Qt::Window);
        //ui->label->showFullScreen();
        this->showFullScreen(); //����ȫ��
        //this->setGeometry(0,0,1440,900);
        ui->label->setGeometry(0,0,1440,900);//ͼƬ�������ó������С
        ui->label->show();//label��ʾ
    }
    ui->lcdNumber->display(t);//��������ʱ
}
