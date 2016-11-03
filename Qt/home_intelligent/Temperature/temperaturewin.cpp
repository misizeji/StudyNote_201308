#include "temperaturewin.h"
#include "ui_temperaturewin.h"

extern "C"  //�Ǵ�д�� C
{
    //#include "init.h"
}

TemperatureWin::TemperatureWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TemperatureWin),bells("./media_sources/warning.wav")
{
    ui->setupUi(this);

    //ȥ���߿�
    setWindowFlags(Qt::FramelessWindowHint);
    //this->showFullScreen();

    //������־λ
    this->sound_tem_flag = 0;
    this->sound_tem_flag_before = 0;
    this->sound_hum_flag = 0;
    this->sound_hum_flag_before = 0;

    //��ʪ�ȳ�ʼֵ
    this->temperature_max_before = 30;
    this->temperature_min_before = 0;
    this->humidity_max_before = 30;
    this->humidity_min_before = 0;
    this->temperature_flag = 0;
    this->humidity_flag = 0;

    //������ʾֵ
    ui->spinBox_TemMax->setValue(temperature_max_before);
    ui->spinBox_TemMin->setValue(temperature_min_before);
    ui->spinBox_WetMax->setValue(humidity_max_before);
    ui->spinBox_WetMin->setValue(humidity_min_before);

    //��ʱ��
    this->timer = new QTimer(this);

    this->timer->start(500);

    //�ź����
    //connect(timer,SIGNAL(timeout()),this,SLOT(TimeSlot()));


}

TemperatureWin::~TemperatureWin()
{
    delete ui;
}


void TemperatureWin::paintEvent(QPaintEvent *)//��ͼ�¼�������ͼƬ
{
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/pic/pic_bg/bkg.png"));
}


void TemperatureWin::TimeSlot()//��ʱ��������
{
    /*
    //��ʪ�ȼ��
    char *Value_recv = init();
    recv_buf = QString(QLatin1String(Value_recv));
    this->Temperature_Value = recv_buf.section(",",0,0);
    this->Humidity_Value = recv_buf.section(",",1,1);
    this->temperature_max = recv_buf.section(",",2,2).toInt();
    this->humidity_max = recv_buf.section(",",3,3).toInt();
    this->temperature_min = recv_buf.section(",",4,4).toInt();
    this->humidity_min = recv_buf.section(",",5,5).toInt();
    this->temperature_flag = recv_buf.section(",",6,6).toInt();
    this->humidity_flag = recv_buf.section(",",7,7).toInt();

    //����Ŀǰ��ʪ��ֵ
    ui->Lb_TemValue->setText(Temperature_Value.left(5));
    ui->Lb_WetValue->setText(Humidity_Value.left(5));

    //�Ա����õ���ʪ��������
    if((temperature_max_before != temperature_max)||
        (temperature_min_before != temperature_min)||
        (humidity_min_before != humidity_min)||
        (humidity_max_before != humidity_max))
    {
        ui->spinBox_TemMin->setValue(temperature_min);
        ui->spinBox_TemMax->setValue(temperature_max);
        ui->spinBox_WetMin->setValue(humidity_min);
        ui->spinBox_WetMax->setValue(humidity_max);

        //�洢��ʪ��������
        temperature_max_before = temperature_max;
        temperature_min_before = temperature_min;
        humidity_min_before = humidity_min;
        humidity_max_before = humidity_max;
    }

    //�¶ȱ�������������
    if(this->temperature_flag == 1)
    {
        ui->Lb_Tem->setPixmap((QPixmap(":/pic/pic_ico/temp_warn.png")));
        QPalette p;	//�½�����
        p.setColor(QPalette::WindowText,Qt::red);	//����������ɫ��ɫ
        ui->Lb_TemValue->setPalette(p);
        this->sound_tem_flag = 1;
    }
    else
    {
        ui->Lb_Tem->setPixmap((QPixmap(":/pic/pic_ico/temp.png")));
        QPalette p;	//�½�����
        p.setColor(QPalette::WindowText,Qt::green);	//����������ɫ��ɫ
        ui->Lb_TemValue->setPalette(p);
        this->sound_tem_flag = 0;
    }

    //ʪ�ȱ�������������
    if(this->humidity_flag == 1)
    {
        ui->Lb_Wet->setPixmap(QPixmap(":/pic/pic_ico/hum_warn.png"));
        QPalette p;	//�½�����
        p.setColor(QPalette::WindowText,Qt::red);	//����������ɫ��ɫ
        ui->Lb_WetValue->setPalette(p);
        this->sound_hum_flag = 1;
    }
    else
    {
        ui->Lb_Wet->setPixmap(QPixmap(":/pic/pic_ico/hum.png"));
        QPalette p;	//�½�����
        p.setColor(QPalette::WindowText,Qt::green);	//����������ɫ��ɫ
        ui->Lb_WetValue->setPalette(p);
        this->sound_hum_flag = 0;
    }

    //��������ⲥ��
    if(((sound_tem_flag != sound_tem_flag_before)||(sound_hum_flag != sound_hum_flag_before))
        &&((sound_tem_flag == 1)||(sound_hum_flag == 1)))
    {
        //bells.setLoops(6);
        //bells.play();
        sound_tem_flag_before = sound_tem_flag;
        sound_hum_flag_before = sound_hum_flag;
    }
    else if(((sound_tem_flag != sound_tem_flag_before)||(sound_hum_flag != sound_hum_flag_before))
        &&((sound_tem_flag == 0)||(sound_hum_flag == 0)))
    {
        //bells.stop();
        sound_tem_flag_before = sound_tem_flag;
        sound_hum_flag_before = sound_hum_flag;
    }
    else
    {
        ;
    }
    */
}

void TemperatureWin::on_Bt_Commit_clicked()//������ʪ��������
{
/*
    QString commit_buf = QString("%1,%2,%3,%4").arg(ui->spinBox_TemMax->value()).arg(ui->spinBox_WetMax->value()).
               arg(ui->spinBox_TemMin->value()).arg(ui->spinBox_WetMin->value());
    QString recv_buf = QString(QLatin1String(real_time(commit_buf.toLatin1().data())));
    this->Temperature_Value = recv_buf.section(",",0,0);
    this->Humidity_Value = recv_buf.section(",",1,1);
    this->temperature_max = recv_buf.section(",",2,2).toInt();
    this->humidity_max = recv_buf.section(",",3,3).toInt();
    this->temperature_min = recv_buf.section(",",4,4).toInt();
    this->humidity_min = recv_buf.section(",",5,5).toInt();
    this->temperature_flag = recv_buf.section(",",6,6).toInt();
    this->humidity_flag = recv_buf.section(",",7,7).toInt();
	
	ui->spinBox_TemMin->setValue(temperature_min);
    ui->spinBox_TemMax->setValue(temperature_max);
    ui->spinBox_WetMin->setValue(humidity_min);
    ui->spinBox_WetMax->setValue(humidity_max);
    //����Ŀǰ��ʪ��ֵ
    ui->Lb_TemValue->setText(Temperature_Value.left(5));
    ui->Lb_WetValue->setText(Humidity_Value.left(5));
*/
}

void TemperatureWin::on_Bt_Exit_clicked()//����������
{
    emit this->show_main_menu();
    this->close();
}
