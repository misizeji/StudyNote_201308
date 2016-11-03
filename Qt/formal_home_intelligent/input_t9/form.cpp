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

extern "C"  //�Ǵ�д�� C
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
    /************��Ӽ���****************/
    put = new T9Widget(ui->widget_2);
    input = new T9Widget(ui->widget_3);
    ui->gridLayout_5->addWidget(put);
    ui->gridLayout->addWidget(input);

    /***********��ʼ����־λ�͵�*************/
    this->timer = new QTimer(this);      //��ʱ��
    //connect(timer,SIGNAL(timeout()),this,SLOT(timeSlot()));
    ui->label_9->setPixmap(QPixmap(":/new/prefix2/off.png"));
    ui->label_10->setPixmap(QPixmap(":/new/prefix2/off.png"));
    ui->label_11->setPixmap(QPixmap(":/new/prefix2/off.png"));
    ui->pushButton_2->setText("ALL ON");

    /******************��������*********************/
    connect(this->put,SIGNAL(newString(QString)),this,SLOT(update_string(QString)));
    connect(this->put,SIGNAL(backSpace()),this,SLOT(backSpaceText()));
    connect(this->input,SIGNAL(newString(QString)),this,SLOT(update_string2(QString)));
    connect(this->input,SIGNAL(backSpace()),this,SLOT(backSpaceText2()));

    /******��ʱ������*********************************/
    //timer->start(500);

    /*******������־λ********/
    this->sound_tem_flag = 0;
    this->sound_tem_flag_before = 0;
    this->sound_hum_flag = 0;
    this->sound_hum_flag_before = 0;

    /******��ʪ�ȳ�ʼֵ��־λ*******/
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
/*******************������ϵ************************/

void Form::update_string(QString str)
{
    if(ui->lineEdit->hasFocus()==true)
    {
        ui->lineEdit->insert(str);  //׷��
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
        ui->textEdit_2->insertPlainText(str);  //׷��
    }
}

void Form::backSpaceText()    //�����˸��
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

void Form::backSpaceText2()    //�����˸��
{
    QString str;
    if(ui->textEdit_2->hasFocus() == true )
    {
        str = ui->textEdit_2->toPlainText(); //���Ȼ�õ��ı����е�����
        if(str.isEmpty() == false)
        {
            str = str.left(str.size()-1);   //�ַ���һ
            ui->textEdit_2->setText(str); //��������
        }
    }
}

/****************���**********************/
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
/*********************�˳�***********************/
/*
void Form::on_pushButton_3_clicked()
{
    this->close();
}
*/
/*********************������*******************/
/*
void Form::on_pushButton_clicked()
{
    QString str_num = ui->lineEdit->text();
    QString str_msg = ui->textEdit->toPlainText();
    gprs_deal(str_num.toLatin1().data(),str_msg.toLatin1().data());
}
*/

void Form::timeSlot()//��ʱ������
{
    //��ʪ�ȼ��
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
        QPalette p;	//�½�����
        p.setColor(QPalette::WindowText,Qt::red);	//����������ɫ��ɫ
        ui->label_16->setPalette(p);
        this->sound_tem_flag = 1;
    }
    else
    {
        ui->label_17->setPixmap((QPixmap(":/new/prefix2/temp.png")));
        QPalette p;	//�½�����
        p.setColor(QPalette::WindowText,Qt::green);	//����������ɫ��ɫ
        ui->label_16->setPalette(p);
        this->sound_tem_flag = 0;
    }
    if(humidity_flag == 1)
    {
        ui->label_19->setPixmap(QPixmap(":/new/prefix2/hum_warn.png"));
        QPalette p;	//�½�����
        p.setColor(QPalette::WindowText,Qt::red);	//����������ɫ��ɫ
        ui->label_18->setPalette(p);
        sound_hum_flag = 1;
    }
    else
    {
        ui->label_19->setPixmap(QPixmap(":/new/prefix2/hum.png"));
        QPalette p;	//�½�����
        p.setColor(QPalette::WindowText,Qt::green);	//����������ɫ��ɫ
        ui->label_18->setPalette(p);
        sound_hum_flag = 0;
    }

    //��������ⲥ��
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

    //LED �����ʾ�л�ͼƬ
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


/*********��ʾ����**************/
/*
void Form::on_pushButton_6_clicked()
{
    msg_get_buf = QString(QLatin1String(msg_get()));
    ui->textEdit_3->setText(msg_get_buf);
}
*/
/**********����*****************/
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
void Form::on_pushButton_4_clicked()//�����¶ȵ�������
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
