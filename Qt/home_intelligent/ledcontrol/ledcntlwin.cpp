#include "ledcntlwin.h"
#include "ui_ledcntlwin.h"

extern "C"  //�Ǵ�д�� C
{
    //#include "set_led.h"
}

LedCntlWin::LedCntlWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LedCntlWin)
{
    ui->setupUi(this);

    //ȥ���߿�
    setWindowFlags(Qt::FramelessWindowHint);
    //this->showFullScreen();

    this->timer = new QTimer(this);      //��ʱ��

    this->timer->start(500);

    //�ź����
    connect(this->timer,SIGNAL(timeout()),this,SLOT(TimeroutSlot()));

}

LedCntlWin::~LedCntlWin()
{
    delete ui;
}

void LedCntlWin::paintEvent(QPaintEvent *)//��ͼ�¼�������ͼƬ
{
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/pic/pic_bg/bkg.png"));
}

void LedCntlWin::on_Bt_Exit_clicked()//�˳���ť
{
    emit this->show_main_menu();
    this->close();
}

void LedCntlWin::on_Bt_Led1_clicked()//led1����
{
    //led_ctl(1);
}

void LedCntlWin::on_Bt_Led2_clicked()//led2����
{
    //led_ctl(2);
}

void LedCntlWin::on_Bt_Led3_clicked()//led3����
{
    //led_ctl(3);
}

void LedCntlWin::on_Bt_LedAll_clicked()//all led����
{
    //led_ctl(4);
}

void LedCntlWin::TimeroutSlot()//��ʱ��������
{
    /*
    //LED �����ʾ�л�ͼƬ
    this->led_state = led_init();

    QIcon icon_ledon;
    icon_ledon.addPixmap(QPixmap(QString::fromUtf8(":/ico/pic_ico/led_on.png")), QIcon::Normal, QIcon::Off);

    QIcon icon_ledoff;
    icon_ledoff.addPixmap(QPixmap(QString::fromUtf8(":/ico/pic_ico/led_off.png")), QIcon::Normal, QIcon::Off);

    QIcon icon_ledalloff;
    icon_ledalloff.addPixmap(QPixmap(QString::fromUtf8(":/ico/pic_ico/alloff.png")), QIcon::Normal, QIcon::Off);

    QIcon icon_ledallon;
    icon_ledallon.addPixmap(QPixmap(QString::fromUtf8(":/ico/pic_ico/allon.png")), QIcon::Normal, QIcon::Off);

    if(led_state == 0)
    {
        ui->Lb_Led1->setPixmap(QPixmap(":/ico/pic_ico/led_1_off.png"));
        ui->Lb_Led2->setPixmap(QPixmap(":/ico/pic_ico/led_2_off.png"));
        ui->Lb_Led3->setPixmap(QPixmap(":/ico/pic_ico/led_3_off.png"));
        ui->Bt_Led1->setIcon(icon_ledoff);
        ui->Bt_Led2->setIcon(icon_ledoff);
        ui->Bt_Led3->setIcon(icon_ledoff);
        ui->Bt_LedAll->setIcon(icon_ledalloff);
    }
    else if(led_state == 1)
    {
        ui->Lb_Led1->setPixmap(QPixmap(":/ico/pic_ico/led_1_off.png"));
        ui->Lb_Led2->setPixmap(QPixmap(":/ico/pic_ico/led_2_off.png"));
        ui->Lb_Led3->setPixmap(QPixmap(":/ico/pic_ico/led_3_on.png"));
        ui->Bt_Led1->setIcon(icon_ledoff);
        ui->Bt_Led2->setIcon(icon_ledoff);
        ui->Bt_Led3->setIcon(icon_ledon);
        ui->Bt_LedAll->setIcon(icon_ledalloff);

    }
    else if(led_state == 10)
    {
        ui->Lb_Led1->setPixmap(QPixmap(":/ico/pic_ico/led_1_off.png"));
        ui->Lb_Led2->setPixmap(QPixmap(":/ico/pic_ico/led_2_on.png"));
        ui->Lb_Led3->setPixmap(QPixmap(":/ico/pic_ico/led_3_off.png"));
        ui->Bt_Led1->setIcon(icon_ledoff);
        ui->Bt_Led2->setIcon(icon_ledon);
        ui->Bt_Led3->setIcon(icon_ledoff);
        ui->Bt_LedAll->setIcon(icon_ledalloff);
    }
    else if(led_state == 11)
    {
        ui->Lb_Led1->setPixmap(QPixmap(":/ico/pic_ico/led_1_off.png"));
        ui->Lb_Led2->setPixmap(QPixmap(":/ico/pic_ico/led_2_on.png"));
        ui->Lb_Led3->setPixmap(QPixmap(":/ico/pic_ico/led_3_on.png"));
        ui->Bt_Led1->setIcon(icon_ledoff);
        ui->Bt_Led2->setIcon(icon_ledon);
        ui->Bt_Led3->setIcon(icon_ledon);
        ui->Bt_LedAll->setIcon(icon_ledalloff);
    }
    else if(led_state == 100)
    {
        ui->Lb_Led1->setPixmap(QPixmap(":/ico/pic_ico/led_1_on.png"));
        ui->Lb_Led2->setPixmap(QPixmap(":/ico/pic_ico/led_2_off.png"));
        ui->Lb_Led3->setPixmap(QPixmap(":/ico/pic_ico/led_3_off.png"));
        ui->Bt_Led1->setIcon(icon_ledon);
        ui->Bt_Led2->setIcon(icon_ledoff);
        ui->Bt_Led3->setIcon(icon_ledoff);
        ui->Bt_LedAll->setIcon(icon_ledalloff);
    }
    else if(led_state == 101)
    {
        ui->Lb_Led1->setPixmap(QPixmap(":/ico/pic_ico/led_1_on.png"));
        ui->Lb_Led2->setPixmap(QPixmap(":/ico/pic_ico/led_2_off.png"));
        ui->Lb_Led3->setPixmap(QPixmap(":/ico/pic_ico/led_3_on.png"));
        ui->Bt_Led1->setIcon(icon_ledon);
        ui->Bt_Led2->setIcon(icon_ledoff);
        ui->Bt_Led3->setIcon(icon_ledon);
        ui->Bt_LedAll->setIcon(icon_ledalloff);
    }
    else if(led_state == 110)
    {
        ui->Lb_Led1->setPixmap(QPixmap(":/ico/pic_ico/led_1_on.png"));
        ui->Lb_Led2->setPixmap(QPixmap(":/ico/pic_ico/led_2_on.png"));
        ui->Lb_Led3->setPixmap(QPixmap(":/ico/pic_ico/led_3_off.png"));
        ui->Bt_Led1->setIcon(icon_ledon);
        ui->Bt_Led2->setIcon(icon_ledon);
        ui->Bt_Led3->setIcon(icon_ledoff);
        ui->Bt_LedAll->setIcon(icon_ledalloff);
    }
    else if(led_state == 111)
    {
        ui->Lb_Led1->setPixmap(QPixmap(":/ico/pic_ico/led_1_on.png"));
        ui->Lb_Led2->setPixmap(QPixmap(":/ico/pic_ico/led_2_on.png"));
        ui->Lb_Led3->setPixmap(QPixmap(":/ico/pic_ico/led_3_on.png"));
        ui->Bt_Led1->setIcon(icon_ledon);
        ui->Bt_Led2->setIcon(icon_ledon);
        ui->Bt_Led3->setIcon(icon_ledon);
        ui->Bt_LedAll->setIcon(icon_ledallon);
    }
*/
}
