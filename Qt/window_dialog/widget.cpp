#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFontDialog>
#include <QColorDialog>
#include <QPalette>
#include <QString>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()//打开文件窗口
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "myOpenFile","../","image(*.png *.jpg *gif)"\
                                                    ";;qt(*.h *.cpp *.ui *.qrc);;all(*.*)");
    if(filename.isEmpty() == false)//文件名是有效的话
    {
        ui->lineEdit->setText(filename);//行编辑进行显示
    }
}

void Widget::on_pushButton_2_clicked()//选取颜色对话框
{
    QColor color = QColorDialog::getColor();
    if(color.isValid() == false)
    {
        return;
    }
	//设置文本编辑颜色
    QPalette p;
    p.setColor(QPalette::Base,color);
    ui->lineEdit_2->setPalette(p);//设置颜色
}

void Widget::on_pushButton_3_clicked()//获得字体
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok);
    if(ok == true)
    {
        ui->lineEdit_3->setFont(font);//行编辑显示选择的字体
    }
}

void Widget::on_pushButton_4_clicked()//消息对话框跳窗
{
	//设置显示信息以及按钮名字
    int button = QMessageBox::question(this,"Ques","are you ok ?",
                          QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    if(button == QMessageBox::Yes)
    {
        ui->label->setText("Yes");
    }
    else if(button == QMessageBox::No)
    {
        ui->label->setText("NO");
    }
    else
    {
        ui->label->setText("Cancel");
    }
}

void Widget::on_pushButton_5_clicked()//信息弹窗
{
    QMessageBox::information(this,"info","Game Over",QMessageBox::Ok);
}

void Widget::on_pushButton_6_clicked()//警告对话框弹窗
{
    QMessageBox::warning(this,"warn","you are going to die!!!",QMessageBox::Ok);
}
