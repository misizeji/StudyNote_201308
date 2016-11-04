#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QString>
#include <QDateTime>
#include <QPixmap>
#include <QPalette>
#include <QStringList>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QStringList>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //��ʼ���Ʒְ�
    ui->MyScore->setPixmap(QPixmap(":/new/num/Numbers/0.png"));
    ui->MyScore->setScaledContents(true);
    ui->OtherScore->setPixmap(QPixmap(":/new/num/Numbers/0.png"));
    ui->OtherScore->setScaledContents(true);

    //�������
    this->music_player = new Phonon::VideoPlayer(Phonon::MusicCategory);
    music_player->play(Phonon::MediaSource("../blackwhite_chess/media_source/dongtian.wav"));
    this->music_player_flag = 1;

    //�������
    this->board = new MyBoard(this);
    ui->gridLayout->addWidget(board,0,1);
    this->board->play_mode = 0;

    //������ͼƬ(��ʼ����ͼƬ������)
    pic_name = ":/new/BG_picture/3d_boy/6.jpg";

    //���ö�ʱ��
    this->timer = new QTimer(this);

    connect(this->board,SIGNAL(update_signal()),this,SLOT(update_score()));//ˢ�¼Ʒְ�
    connect(this->board,SIGNAL(update_time_signal()),this,SLOT(update_time()));//ˢ�µ���ʱ����,��ʼ��ʱ
    connect(timer,SIGNAL(timeout()),this,SLOT(lcd_time_show()));//lcd����ʱ����ʾ
    connect(this->board,SIGNAL(game_over()),this,SLOT(game_over_judge()));  //�ж���Ӯ

}

Widget::~Widget()
{
    delete ui;
    delete board;
    delete timer;
    delete music_player;
}

void Widget::paintEvent(QPaintEvent *)//��ͼ�¼�������ͼƬ
{
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap(pic_name));
}

void Widget::on_button_ChangeBG_clicked()//�ı䱳��
{
    static int i_before = 0;   //����ͼƬ�л������洢i
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));  //����ʱ������
    int t = qrand();   //��ȡ��ǰʱ������
    int i = (t%17);     //��ȡ0-16֮�������
    if(i == i_before)
    {
       i = i + 1;      //���i=4�Ļ���i+1����5������ǡ���������6��ͼƬ
    }
    QStringList pic_list;   //�����ַ���ͼƬ����
    pic_list<<":/new/BG_picture/3d_boy/1.jpg"<<":/new/BG_picture/3d_boy/2.jpg"<<":/new/BG_picture/3d_boy/3.jpg"
            <<":/new/BG_picture/3d_boy/4.jpg"<<":/new/BG_picture/3d_boy/5.jpg"<<":/new/BG_picture/3d_boy/6.jpg"
            <<":/new/BG_picture/3d_boy/7.jpg"<<":/new/BG_picture/3d_boy/8.jpg"<<":/new/BG_picture/3d_boy/9.jpg"
            <<":/new/BG_picture/3d_boy/10.jpg"<<":/new/BG_picture/3d_boy/11.jpg"<<":/new/BG_picture/3d_boy/12.jpg"
            <<":/new/BG_picture/3d_boy/13.jpg"<<":/new/BG_picture/3d_boy/14.jpg"<<":/new/BG_picture/3d_boy/15.jpg"
            <<":/new/BG_picture/3d_boy/16.jpg"<<":/new/BG_picture/3d_boy/17.jpg"<<":/new/BG_picture/3d_boy/0.jpg";
    this->pic_name = pic_list[i];   //�޸Ļ�ͼͼƬ������
    i_before = i;
    this->update();    //ˢ�»�ͼ
}

void Widget::on_toolButton_music_clicked()//���ֲ���
{
    if(this->music_player_flag == 1)
    {
        this->music_player->pause();
        this->music_player_flag = 0;
    }
    else
    {
        music_player->play(Phonon::MediaSource("../blackwhite_chess/media_source/dongtian.wav"));
        this->music_player_flag = 1;
    }
}

void Widget::on_WatchMovie_clicked()//�ۿ���Ƶ
{
    //��Ӳ�����
    this->Player = new MyPlayer;
    this->Player->move(385,70);
    this->Player->show();
}

void Widget::on_toolButton_PVP_clicked()//���˶�ս
{
    this->board->game_init();//��ʼ������
    this->board->play_mode = 1;//������Ϸģʽ
    set_time_count();   //���ö�ʱ��
    ui->toolButton_PVP->setEnabled(false);
    ui->toolButton_PVC->setEnabled(false);
    ui->toolButton_pvnet->setEnabled(false);
}

void Widget::on_toolButton_PVC_clicked()//�˻���ս
{
    this->board->game_init();//��ʼ������
    this->board->play_mode = 2;//������Ϸģʽ
    set_time_count();   //���ö�ʱ��
    ui->toolButton_PVP->setEnabled(false);
    ui->toolButton_PVC->setEnabled(false);
    ui->toolButton_pvnet->setEnabled(false);
}

void Widget::on_toolButton_pvnet_clicked()//�����ս����
{
    this->board->game_init();//��ʼ������
    this->board->play_mode = 3;//������Ϸģʽ
    this->board->link_pvnet();//�����Ӵ���
    ui->toolButton_PVP->setEnabled(false);
    ui->toolButton_PVC->setEnabled(false);
    ui->toolButton_pvnet->setEnabled(false);
    ui->toolButton_Regret->setEnabled(false);
}

void Widget::on_toolButton_Regret_clicked()//����
{
    if(this->board->play_mode == 0)//δ��ʼ����Ϸ����
    {
        return;
    }
    else
    {
        this->board->set_put_before(this->board->put_times-1);
    }
}

void Widget::update_score()//ˢ�¼Ʒְ庯��
{
    QString mycount_str = QString(":/new/num/Numbers/%1.png").arg(board->get_count(1));
    QString othercount_str = QString(":/new/num/Numbers/%1.png").arg(board->get_count(2));
    ui->MyScore->setPixmap(QPixmap(mycount_str));
    ui->OtherScore->setPixmap(QPixmap(othercount_str));

    //��Ϸ������������
    if((board->get_count(1) + board->get_count(2)) == 64)
    {
        game_over_judge();//�ж���Ӯ
    }
}

void Widget::set_time_count()//����ʱ��
{
    this->play_time = 30;
    ui->lcdNumber_time->display(play_time);
    this->timer->start(1000);
}

void Widget::update_time()//ˢ�µ���ʱ����,��ʼ��ʱ
{
    //qDebug()<<"in the update_time";
    this->timer->stop();//��ʱ������
    set_time_count();   //���ö�ʱ��
}

void Widget::lcd_time_show()//lcd��ʱ��ʾ
{
    //qDebug()<<"in the lcd_time_show";
    this->play_time--;
    if(this->play_time == 0)
    {
        if(this->board->play_mode == 3 && this->board->Player_flag == this->board->my_flag)
        {
            this->board->msg_send(10,0,this->board->my_flag);//����Է���Ӧ
            this->board->mouse_flag = 0;//�����Լ������
            this->play_time = 30;
            ui->lcdNumber_time->display(this->play_time);
            return;
        }

        this->board->player_flag_change();//����ѡ��
        this->play_time = 30;
        //�˲���(ʱ���ȥ��)���û���������
        if(this->board->play_mode == 2 && this->board->Player_flag == 2)//ģʽ���˻����ֵ�����������
        {
            this->board->pvc_game_mode(3,4);//������֪���ӵ����꣬ʹ�������¶���������(3,4)
        }
    }
    ui->lcdNumber_time->display(this->play_time);
}

void Widget::game_over_judge()//��Ϸ������Ӯ�ж�
{
    this->board->play_mode = 0;//���̽��������Ӧ
    this->timer->stop();//��ʱ������
    ui->lcdNumber_time->display(0);//����ʱ��ʾ0

    if(board->get_count(1) == board->get_count(2))
    {
        QMessageBox::information(this,"info","Game Over\nboth win!!!",QMessageBox::Ok);
    }
    else if(board->get_count(1) > board->get_count(2))
    {
        QMessageBox::information(this,"info","Game Over\nwhite win!!!",QMessageBox::Ok);
    }
    else if(board->get_count(1) < board->get_count(2))
    {
        QMessageBox::information(this,"info","Game Over\nblack win!!!",QMessageBox::Ok);
    }
    ui->toolButton_PVP->setEnabled(true);
    ui->toolButton_PVC->setEnabled(true);
    ui->toolButton_pvnet->setEnabled(true);
    ui->toolButton_Regret->setEnabled(true);
}

void Widget::on_toolButton_clicked()//�Զ��طź���
{
    this->board->refresh();
}

void Widget::on_toolButton_tips_clicked()//����λ�����Ѻ���
{
    if(this->board->play_mode == 0)
        return;
    QString tips = this->board->put_tips();
    ui->label_tipshow->setText(tips);
}

void Widget::on_toolButton_talk_clicked()//��������
{
    this->mytalk = new udp_talk;
    this->mytalk->show();
}
