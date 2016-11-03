#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QMovie>
#include <QFont>
#include <QTime>
#include <QDate>
#include <QDateTime>
#include <QPalette>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->pushButton->hide();		//设置PushBotton隐藏
    ui->pushButton->show();		//显示PushButton
    int w = this->width();		//获得当前窗口的宽度
    int h = this->height();		//获得当前窗口的高度
    qDebug()<<"this W = "<<w
            <<"this H = "<<h;
    int bw = ui->pushButton->width();	//获得PushBotton的宽度
    int bh = ui->pushButton->height();	//获得PushBotton的高度
    qDebug()<<"this W = "<<bw
            <<"this H = "<<bh;
    ui->pushButton->resize(100,30);		//重新设置PushButton的大小
    qDebug()<<"bw = "<<ui->pushButton->width()
            <<" bh = "<<ui->pushButton->height()
            <<", size = "<<ui->pushButton->size();
    this->move(100,50);		//移动窗口出现的初始位置
    qDebug()<<"this x ="<<this->x()	//当前窗口的x坐标
            <<" this y = "<<this->y()//当前窗口的y坐标
            <<",this pos = "<<this->pos();//当前窗口的位置
    ui->pushButton->setGeometry(50,20,101,30);	//设置PushButton的起始位置与大小
    qDebug()<<"button "<<ui->pushButton->geometry(); //获取PushBotton的位置与大小信息
    ui->pushButton->setEnabled(false);	//设置PushButton不能被按下
    qDebug()<<"button "<<ui->pushButton->isEnabled();	//查看PushButton能不能按下

    QString str1 = "63";	//设置一个字符串
    int n = str1.toInt();	//将字符串转换成int数
    qDebug()<<"n = "<<n;	
	
    n = 20;
    str1 = QString::number(n,16);	//整数转换成字符串，这里是转换成16进制
    qDebug()<<"str1 = "<<str1;
	//组包这里arg既可以是数字还可以是字符串
    QString str2 = QString("aa%1 %2bb,%3").arg(str1).arg(n).arg("haha");
    qDebug()<<"str2 = "<<str2;

    str1 = "aa:bb:cc:dd:ee";
	//字符串拆包操作
    str2 = str1.section(":",1,1);//取第一个“:”之后的一个字符串"bb"
    qDebug()<<"str2 = "<<str2;

	//字符串组包操作
    QStringList list;	//建立一个字符串数组	
    list<<str2<<"aa"<<"bb";	//将之前拆包的str2:"bb",与"aa","bb"进行组包
    for(int i = 0;i < list.size();i++)
    {
        QString str = list[i];
        qDebug()<<i<<","<<str;
    }

    qDebug()<<"list contains aa="<<list.contains("qq");		//判断字符串中是否含有字符串"qq"
    qDebug()<<"list contains aa="<<list.contains("aa");		//判断字符串中是否含有字符串"aa"
	

    str2 = list.join(":");		//在字符串数组中每个字符串之间添加一个":"
    qDebug()<<"str2 = "<<str2;
    str2 = "11:22:33:44:55";
    list = str2.split(":");		//字符串以:分割成多个字符串，形成字符串数组
	//打印出字符串数组中字符串的个数，将字符串数组打印出来
    qDebug()<<"list size="<<list.size()<<"\n"<<list;	

    ui->label->setText("hello Qt");		//设置标签显示字符
    ui->label->setPixmap(QPixmap("../image/network-globe2.png"));//设置标签图片
    ui->label->setScaledContents(true);		//设置标签图片自动适应
    qDebug()<<"label text = "<<ui->label->text();	//获取标签内容

    QMovie *movie = new QMovie(this);	//给动画设置空间
    movie->setFileName("../image/boy.gif");	//设置动画

    ui->label_2->setMovie(movie);	//将动画设置在标签2中
    movie->start();	//开启动画
    ui->label_2->setScaledContents(true);	//设置动画自动适应大小

    ui->lcdNumber->setDigitCount(4);	//设置显示的位数
    ui->lcdNumber->display(3.14);		//设置显示的数字

    ui->progressBar->setMinimum(0);		//设置进度条的最小值
    ui->progressBar->setMaximum(200);	//设置进度条的最大值
    ui->progressBar->setValue(100);		//设置进度条的当前值

    ui->lineEdit->setText("123456");	//设置行编辑内容
    ui->lineEdit->insert("abc");		//插入"abc"到行编辑的尾部
    ui->lineEdit->setEchoMode(QLineEdit::Password);		//设置编辑显示模式
    qDebug()<<ui->lineEdit->text();		//获得行编辑内容

    ui->textEdit->setFontPointSize(10);		//设置字体大小
    ui->textEdit->setTextColor(QColor(255,0,0));	//设置字体颜色
    ui->textEdit->setText("abcde");				//设置文本编辑的内容
    ui->textEdit->setFontPointSize(15);			//重新设置字体的大小
    ui->textEdit->setTextColor(Qt::cyan);		//设置颜色为cyan
    ui->textEdit->append("你好");				//下行显示"你好"
	qDebug()<<ui->textEdit->toPlainText();		//获取文本编辑框中的内容

    ui->comboBox->setEditable(true);			//设置下拉框可编辑
    ui->comboBox->addItem("linux 基础");		//添加条目"linux基础"
    ui->comboBox->addItem(QIcon("../image/myheart.png"),"Qt");//添加条目+图标
    ui->comboBox->insertItem(2,"C++");	//插入条目"C++"到第二位（从第零位开始数）
    qDebug()<<ui->comboBox->count();	//获取现在条目的数量
    ui->comboBox->setCurrentIndex(2);	//设置初始化显示的条目是第2个条目
    qDebug()<<ui->comboBox->currentText()<<", "
            <<ui->comboBox->currentIndex();		//获取当前的条目与当前条目占总条目中的位置
    ui->pushButton_2->setText("yes");		//设置pushbutton2显示内容
    ui->toolButton->setText("haha");		//设置toolbutton显示内容
    ui->radioButton->setText("heihei");		//设置radiobutton显示的内容
    ui->checkBox->setText("哈哈");			//显示复选框显示的内容
    qDebug()<<"check box = "<<ui->checkBox->text();	//获得复选框显示内容
    ui->toolButton->setIcon(QIcon(":/new/prefix1/face-1.png"));	//设置toolButton背景图片
    ui->toolButton->resize(40,60);	//设置toolButton的大小
    ui->toolButton->setIconSize(QSize(ui->toolButton->width(),60));//设置图片大小
    ui->toolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);//设置文字在工具按钮图片下面

    ui->radioButton->setIcon(QIcon("../image/wright.png"));//设置radioButton图标
    ui->radioButton->setIconSize(QSize(20,20));//设置radioButton的图标大小

    ui->radioButton->setChecked(true);//设置单选框选中状态
    qDebug()<<ui->radioButton->isChecked();	//获取radioButton是否被选中

	//新建字体
    QFont font;
    font.setFamily("幼圆");
    font.setPointSize(20);
    font.setBold(true);
    font.setUnderline(true);
    ui->textEdit->setFont(font);

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));	//获取当前时间种子
    int t = qrand();
    ui->lcdNumber->setDigitCount(QString::number(t).size());//设置lcd显示的位数
    ui->lcdNumber->display(t);//lcd显示时间种子

    QString time = QTime::currentTime().toString("hh:mm:ss");//将当前时间转换成字符串

    ui->textEdit->setText(time);//文本编辑框显示时间字符串
    ui->textEdit->append(QDate::currentDate().toString("yyyy-MM-dd"));//日期转换字符串

    QPalette p;	//新建配置
    //p.setColor(QPalette::Window,Qt::green);
    p.setColor(QPalette::WindowText,Qt::magenta);	//设置字体颜色粉色
    p.setBrush(QPalette::Window,QBrush(QPixmap(":/new/prefix1/bk.jpg")));//设置背景图片
    ui->lcdNumber->setAutoFillBackground(true);	//自动填满背景
    ui->lcdNumber->setPalette(p);	//lcd配置Palette	
}
Widget::~Widget()
{
    delete ui;
}
