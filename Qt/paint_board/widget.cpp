#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QColor>
#include <QFileDialog>
#include <QColorDialog>
#include <QPalette>
#include <QString>
#include <QMouseEvent>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setMouseTracking(true);
    ui->lineEdit->setEnabled(false);    //�ı��б༭ʧЧ

    this->area = new ScribbleArea;      //�����µĻ�ͼ��
    ui->gridLayout->addWidget(area,1,0);    //��ӻ�ͼ�崰��
    mycolor = new QColor;
    *mycolor = this->area->penColor();      //��ȡ��ǰ������ɫ
    //qDebug()<<"color"<<mycolor;

    //�����б༭��ɫ
    QPalette bk;
    bk.setColor(QPalette::Base,*mycolor);
    ui->lineEdit->setPalette(bk);

    int penwidth = this->area->penWidth();//��ȡ���ʿ��
    ui->spinBox->setValue(penwidth);    //���û��ʿ����ʾ

}

Widget::~Widget()
{
    delete ui;
    delete area;
    delete mycolor;
}

void Widget::on_pushButton_clicked()//������ť
{
    this->area->clearImage();
}

void Widget::on_pushButton_2_clicked()//��ͼƬ
{
    QString str = QFileDialog::getOpenFileName(this,
                                               "myOpenFile","../","image(*.png *.jpg *gif)"\
                                               ";;qt(*.h *.cpp *.ui *.qrc);;all(*.*)");
    if(str != NULL)
    {
        this->area->openImage(str);
    }
}

void Widget::on_pushButton_3_clicked()//�ر��˳�
{
    this->close();
}

void Widget::on_spinBox_valueChanged(int a)//���û��ʿ��
{
    //qDebug()<<"a = "<<a;
    this->area->setPenWidth(a);
}

void Widget::on_toolButton_clicked()    //ѡ�񻭱���ɫ��������
{
    QColor p = QColorDialog::getColor();
    //qDebug()<<"get color"<<p;
    if(p.isValid() == false)
    {
        return;
    }
    QPalette base;
    base.setColor(QPalette::Base,p);
    ui->lineEdit->setPalette(base);
    this->area->setPenColor(p);//������ɫ
}

void Widget::mouseMoveEvent(QMouseEvent *ev)//���������״
{
    //qDebug()<<ev->pos();
    if((ev->x() > 11)&&(ev->x()>(11+this->area->x())) &&
       (ev->y() > 47)&&(ev->y()>(47+this->area->y())))
    {
        this->area->setCursor(Qt::CrossCursor);
    }
}
