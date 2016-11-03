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
    ui->lineEdit->setEnabled(false);    //文本行编辑失效

    this->area = new ScribbleArea;      //建立新的画图板
    ui->gridLayout->addWidget(area,1,0);    //添加画图板窗口
    mycolor = new QColor;
    *mycolor = this->area->penColor();      //获取当前画笔颜色
    //qDebug()<<"color"<<mycolor;

    //设置行编辑底色
    QPalette bk;
    bk.setColor(QPalette::Base,*mycolor);
    ui->lineEdit->setPalette(bk);

    int penwidth = this->area->penWidth();//获取画笔宽度
    ui->spinBox->setValue(penwidth);    //设置画笔宽度显示

}

Widget::~Widget()
{
    delete ui;
    delete area;
    delete mycolor;
}

void Widget::on_pushButton_clicked()//清屏按钮
{
    this->area->clearImage();
}

void Widget::on_pushButton_2_clicked()//打开图片
{
    QString str = QFileDialog::getOpenFileName(this,
                                               "myOpenFile","../","image(*.png *.jpg *gif)"\
                                               ";;qt(*.h *.cpp *.ui *.qrc);;all(*.*)");
    if(str != NULL)
    {
        this->area->openImage(str);
    }
}

void Widget::on_pushButton_3_clicked()//关闭退出
{
    this->close();
}

void Widget::on_spinBox_valueChanged(int a)//设置画笔宽度
{
    //qDebug()<<"a = "<<a;
    this->area->setPenWidth(a);
}

void Widget::on_toolButton_clicked()    //选择画笔颜色并且设置
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
    this->area->setPenColor(p);//设置颜色
}

void Widget::mouseMoveEvent(QMouseEvent *ev)//设置鼠标形状
{
    //qDebug()<<ev->pos();
    if((ev->x() > 11)&&(ev->x()>(11+this->area->x())) &&
       (ev->y() > 47)&&(ev->y()>(47+this->area->y())))
    {
        this->area->setCursor(Qt::CrossCursor);
    }
}
