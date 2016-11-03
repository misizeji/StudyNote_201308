#include "mainmenu.h"
#include "ui_mainmenu.h"


#include <QPainter>

MainMenu::MainMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);

    //ȥ���߿�
    setWindowFlags(Qt::FramelessWindowHint);
    //this->showFullScreen();

    //����û�����˵�
    this->UsrMng = new UserMngWin;
    //���led�ƿ��ƴ���
    this->LedCntl = new LedCntlWin;
    //���GPRSģ��Զ�̷��Ͷ���
    this->GprsMsg = new GPRSMsgWin;
    //�����ʪ�ȿ��Ʋ˵�����
    this->TemCntl = new TemperatureWin;
    //��ӱ������Դ���
    this->MsgLeave = new MsgLeaveWin;
    //��ӵ�Ӱ����������
    this->MovieWin = new moviewin;
    //������ô���
    this->Setup = new SetupWin;

    //�ź���� ��ʾ���˵�
    connect(this->UsrMng,SIGNAL(show_main_menu()),this,SLOT(main_menu_show()));
    connect(this->LedCntl,SIGNAL(show_main_menu()),this,SLOT(main_menu_show()));
    connect(this->GprsMsg,SIGNAL(show_main_menu()),this,SLOT(main_menu_show()));
    connect(this->TemCntl,SIGNAL(show_main_menu()),this,SLOT(main_menu_show()));
    connect(this->MsgLeave,SIGNAL(show_main_menu()),this,SLOT(main_menu_show()));
    connect(this->MovieWin,SIGNAL(show_main_menu()),this,SLOT(main_menu_show()));
    connect(this->Setup,SIGNAL(show_main_menu()),this,SLOT(main_menu_show()));


}

MainMenu::~MainMenu()
{
    delete ui;
}


void MainMenu::main_menu_show()//��ʾ���˵�
{
    this->show();      //��ʾ���˵�����
}

void MainMenu::on_Bt_Exit_clicked()//�˳��˵�
{
    emit menu_exit();   //�˵��˳�
    this->close();      //�رձ�����
}

void MainMenu::on_Bt_UserMng_clicked()//�û�����˵�
{
    this->close();
    this->UsrMng->show();
}

void MainMenu::on_Bt_LedSet_clicked()//led�ƿ���
{
    this->close();
    this->LedCntl->show();
}

void MainMenu::on_Bt_GPRSMsg_clicked()//GPRSԶ�̷��Ͷ���
{
    this->close();
    this->GprsMsg->show();
}

void MainMenu::on_Bt_Tem_clicked()//��ʪ�ȿ��ƴ���
{
    this->close();
    this->TemCntl->show();
}

void MainMenu::on_Bt_MsgLeave_clicked()//���Դ���
{
    this->close();
    this->MsgLeave->show();
}

void MainMenu::on_Bt_Movie_clicked()//��Ӱ���Ŵ���
{
    this->close();
    this->MovieWin->show();
    this->MovieWin->movie_begin();
}

void MainMenu::on_Bt_Setup_clicked()
{
    this->close();
    this->Setup->show();
}
