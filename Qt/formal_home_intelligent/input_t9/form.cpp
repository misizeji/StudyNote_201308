#include "form.h"
#include "ui_form.h"
#include "t9widget.h"
#include <QPixmap>
#include <QDebug>
#include <QByteArray>
#include <QString>
#include <QPalette>
#include <QColor>
#include <QFont>
#include <QSound>
#include <QMessageBox>

extern "C"  //是大写的 C
{
    /*
    #include "set_led.h"
    #include "key_deal.h"
    #include "gprs.h"
    #include "sqlite3.h"
    #include "init.h"
    #include "msgget.h"
    #include "msgsnd.h"
    */
}

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form),bells("../music/firetrucksiren.wav")
{
    ui->setupUi(this);
    /************添加键盘****************/
    put = new T9Widget(ui->widget_2);
    input = new T9Widget(ui->widget_3);
    ui->gridLayout_5->addWidget(put);
    ui->gridLayout->addWidget(input);

    /***********初始化标志位和灯*************/
    this->timer = new QTimer(this);      //定时器
    //connect(timer,SIGNAL(timeout()),this,SLOT(timeSlot()));
    ui->label_9->setPixmap(QPixmap(":/new/prefix2/off.png"));
    ui->label_10->setPixmap(QPixmap(":/new/prefix2/off.png"));
    ui->label_11->setPixmap(QPixmap(":/new/prefix2/off.png"));
    ui->pushButton_2->setText("ALL ON");

    /******************键盘连接*********************/
    connect(this->put,SIGNAL(newString(QString)),this,SLOT(update_string(QString)));
    connect(this->put,SIGNAL(backSpace()),this,SLOT(backSpaceText()));
    connect(this->input,SIGNAL(newString(QString)),this,SLOT(update_string2(QString)));
    connect(this->input,SIGNAL(backSpace()),this,SLOT(backSpaceText2()));

    /******定时器启动*********************************/
    //timer->start(500);

    /*******报警标志位********/
    this->sound_tem_flag = 0;
    this->sound_tem_flag_before = 0;
    this->sound_hum_flag = 0;
    this->sound_hum_flag_before = 0;

    /******温湿度初始值标志位*******/
    temperature_max_before = 30;
    humidity_max_before = 30;
    temperature_min_before = 0;
    humidity_min_before = 0;
    ui->spinBox->setValue(temperature_min_before);
    ui->spinBox_2->setValue(temperature_max_before);
    ui->spinBox_3->setValue(humidity_min_before);
    ui->spinBox_4->setValue(humidity_max_before);

}

Form::~Form()
{
    delete ui;
}
/*******************键盘联系************************/

void Form::update_string(QString str)
{
    if(ui->lineEdit->hasFocus()==true)
    {
        ui->lineEdit->insert(str);  //追加
    }
    if(ui->textEdit->hasFocus()==true)
    {
        ui->textEdit->insertPlainText(str);
    }
}

void Form::update_string2(QString str)
{
    if(ui->textEdit_2->hasFocus()==true)
    {
        ui->textEdit_2->insertPlainText(str);  //追加
    }
}

void Form::backSpaceText()    //按了退格键
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
    else if(ui->textEdit->hasFocus() == true)
    {
        str = ui->textEdit->toPlainText();
        if(str.isEmpty() == false)
        {
            str = str.left(str.size()-1);
            ui->textEdit->setText(str);
        }
    }
}

void Form::backSpaceText2()    //按了退格键
{
    QString str;
    if(ui->textEdit_2->hasFocus() == true )
    {
        str = ui->textEdit_2->toPlainText(); //首先获得到文本框中的内容
        if(str.isEmpty() == false)
        {
            str = str.left(str.size()-1);   //字符减一
            ui->textEdit_2->setText(str); //重新设置
        }
    }
}

/****************点灯**********************/
/*
void Form::on_toolButton_clicked()
{
    led_ctl(1);
}

void Form::on_toolButton_2_clicked()
{
    led_ctl(2);
}

void Form::on_toolButton_3_clicked()
{
    led_ctl(3);
}

void Form::on_pushButton_2_clicked()
{
   led_ctl(4);
}
*/
/*********************退出***********************/
/*
void Form::on_pushButton_3_clicked()
{
    this->close();
}
*/
/*********************发短信*******************/
/*
void Form::on_pushButton_clicked()
{
    QString str_num = ui->lineEdit->text();
    QString str_msg = ui->textEdit->toPlainText();
    gprs_deal(str_num.toLatin1().data(),str_msg.toLatin1().data());
}
*/

void Form::timeSlot()//定时器函数
{
    //温湿度检测
    char *u = init();
    buf = QString(QLatin1String(u));
    temperature = buf.section(",",0,0);
    humidity = buf.section(",",1,1);
    temperature_max = buf.section(",",2,2).toInt();
    humidity_max = buf.section(",",3,3).toInt();
    temperature_min = buf.section(",",4,4).toInt();
    humidity_min = buf.section(",",5,5).toInt();
    temperature_flag = buf.section(",",6,6).toInt();
    humidity_flag = buf.section(",",7,7).toInt();

    ui->label_16->setText(temperature.left(5));
    ui->label_18->setText(humidity.left(5));
    if((temperature_max_before != temperature_max)||
        (temperature_min_before != temperature_min)||
        (humidity_min_before != humidity_min)||
        (humidity_max_before != humidity_max))
    {
        ui->spinBox->setValue(temperature_min);
        ui->spinBox_2->setValue(temperature_max);
        ui->spinBox_3->setValue(humidity_min);
        ui->spinBox_4->setValue(humidity_max);
        temperature_max_before = temperature_max;
        temperature_min_before = temperature_min;
        humidity_min_before = humidity_min;
        humidity_max_before = humidity_max;
    }

    if(temperature_flag == 1)
    {
        ui->label_17->setPixmap((QPixmap(":/new/prefix2/temp_warn.png")));
        QPalette p;	//新建配置
        p.setColor(QPalette::WindowText,Qt::red);	//设置字体颜色粉色
        ui->label_16->setPalette(p);
        this->sound_tem_flag = 1;
    }
    else
    {
        ui->label_17->setPixmap((QPixmap(":/new/prefix2/temp.png")));
        QPalette p;	//新建配置
        p.setColor(QPalette::WindowText,Qt::green);	//设置字体颜色粉色
        ui->label_16->setPalette(p);
        this->sound_tem_flag = 0;
    }
    if(humidity_flag == 1)
    {
        ui->label_19->setPixmap(QPixmap(":/new/prefix2/hum_warn.png"));
        QPalette p;	//新建配置
        p.setColor(QPalette::WindowText,Qt::red);	//设置字体颜色粉色
        ui->label_18->setPalette(p);
        sound_hum_flag = 1;
    }
    else
    {
        ui->label_19->setPixmap(QPixmap(":/new/prefix2/hum.png"));
        QPalette p;	//新建配置
        p.setColor(QPalette::WindowText,Qt::green);	//设置字体颜色粉色
        ui->label_18->setPalette(p);
        sound_hum_flag = 0;
    }

    //报警音检测播放
    if(((sound_tem_flag != sound_tem_flag_before)||(sound_hum_flag != sound_hum_flag_before))
        &&((sound_tem_flag == 1)||(sound_hum_flag == 1)))
    {
        bells.setLoops(6);
        bells.play();
        sound_tem_flag_before = sound_tem_flag;
        sound_hum_flag_before = sound_hum_flag;
    }
    else if(((sound_tem_flag != sound_tem_flag_before)||(sound_hum_flag != sound_hum_flag_before))
        &&((sound_tem_flag == 0)||(sound_hum_flag == 0)))
    {
        bells.stop();
        sound_tem_flag_before = sound_tem_flag;
        sound_hum_flag_before = sound_hum_flag;
    }
    else
    {
        ;
    }

    //LED 检测显示切换图片
    this->led_state = led_init();
    if(led_state == 0)
    {
        ui->label_9->setPixmap(QPixmap(":/new/prefix2/off.png"));
        ui->label_10->setPixmap(QPixmap(":/new/prefix2/off.png"));
        ui->label_11->setPixmap(QPixmap(":/new/prefix2/off.png"));
        ui->pushButton_2->setText("ALL OFF");
    }
    else if(led_state == 1)
    {
        ui->label_9->setPixmap(QPixmap(":/new/prefix2/off.png"));
        ui->label_10->setPixmap(QPixmap(":/new/prefix2/off.png"));
        ui->label_11->setPixmap(QPixmap(":/new/prefix2/on.png"));
    }
    else if(led_state == 10)
    {
        ui->label_9->setPixmap(QPixmap(":/new/prefix2/off.png"));
        ui->label_10->setPixmap(QPixmap(":/new/prefix2/on.png"));
        ui->label_11->setPixmap(QPixmap(":/new/prefix2/off.png"));
    }
    else if(led_state == 11)
    {
        ui->label_9->setPixmap(QPixmap(":/new/prefix2/off.png"));
        ui->label_10->setPixmap(QPixmap(":/new/prefix2/on.png"));
        ui->label_11->setPixmap(QPixmap(":/new/prefix2/on.png"));
    }
    else if(led_state == 100)
    {
        ui->label_9->setPixmap(QPixmap(":/new/prefix2/on.png"));
        ui->label_10->setPixmap(QPixmap(":/new/prefix2/off.png"));
        ui->label_11->setPixmap(QPixmap(":/new/prefix2/off.png"));
    }
    else if(led_state == 101)
    {
        ui->label_9->setPixmap(QPixmap(":/new/prefix2/on.png"));
        ui->label_10->setPixmap(QPixmap(":/new/prefix2/off.png"));
        ui->label_11->setPixmap(QPixmap(":/new/prefix2/on.png"));
    }
    else if(led_state == 110)
    {
        ui->label_9->setPixmap(QPixmap(":/new/prefix2/on.png"));
        ui->label_10->setPixmap(QPixmap(":/new/prefix2/on.png"));
        ui->label_11->setPixmap(QPixmap(":/new/prefix2/off.png"));
    }
    else if(led_state == 111)
    {
        ui->label_9->setPixmap(QPixmap(":/new/prefix2/on.png"));
        ui->label_10->setPixmap(QPixmap(":/new/prefix2/on.png"));
        ui->label_11->setPixmap(QPixmap(":/new/prefix2/on.png"));
        ui->pushButton_2->setText("ALL ON");
    }

}


/*********显示留言**************/
/*
void Form::on_pushButton_6_clicked()
{
    msg_get_buf = QString(QLatin1String(msg_get()));
    ui->textEdit_3->setText(msg_get_buf);
}
*/
/**********留言*****************/
/*
void Form::on_pushButton_5_clicked()
{
   int ret = msg_send(ui->textEdit_2->toPlainText().toAscii().data());
   ui->textEdit_2->setText("");

   if(ret == 0)
   {
       QMessageBox::warning(this,"congratulation"," successful!!");
   }
   else
   {
       QMessageBox::warning(this,"congratulation"," failed!!");
   }
}
*/


/*
void Form::on_pushButton_4_clicked()//设置温度的上下限
{
    sbuf = QString("%1,%2,%3,%4").arg(ui->spinBox_2->value()).arg(ui->spinBox_4->value()).
           arg(ui->spinBox->value()).arg(ui->spinBox_3->value());
    buf = QString(QLatin1String(real_time(sbuf.toLatin1().data())));
    //qDebug()<<"buf = "<<buf;
    temperature = buf.section(",",0,0);
    humidity = buf.section(",",1,1);
    temperature_max = buf.section(",",2,2).toInt();
    humidity_max = buf.section(",",3,3).toInt();
    temperature_min = buf.section(",",4,4).toInt();
    humidity_min = buf.section(",",5,5).toInt();
    temperature_flag = buf.section(",",6,6).toInt();
    humidity_flag = buf.section(",",7,7).toInt();
    ui->spinBox->setValue(temperature_min);
    ui->spinBox_2->setValue(temperature_max);
    ui->spinBox_3->setValue(humidity_min);
    ui->spinBox_4->setValue(humidity_max);
    ui->label_16->setText(temperature.left(5));
    ui->label_18->setText(humidity.left(5));
}
*/
