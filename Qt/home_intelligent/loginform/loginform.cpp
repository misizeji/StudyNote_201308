#include "loginform.h"
#include "ui_loginform.h"


#include <QPainter>
#include <QString>
#include <QDebug>
#include <QTimer>
#include <QPaintEvent>
#include <QMessageBox>


//C++�в���C���Եķ�ʽ
extern "C"  //�Ǵ�д�� C
{
/*
    #include "./account_register.h"
    #include "./normal_login.h"
    #include "./database/sqlite3.h"
*/
}


LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);

    //ȥ���߿�
    setWindowFlags(Qt::FramelessWindowHint);
    //this->showFullScreen();

    //���ö�ʱ��
    this->timer = new QTimer(this);
    this->timer->start(1000*show_menu_delay);

    //��Ӽ���
    this->input = new T9Widget(ui->Edit_usrName,this);
    this->input->move(450,30);
    this->input->show();

    //������˵�
    this->mainmenu = new MainMenu;
    //�����Ϣ������ʾ����
    this->msgshow = new MsgShowWin;

    ui->Edit_usrName->setFocus();

    //������ϵ �ź����
    connect(this->input,SIGNAL(newString(QString)),this,SLOT(EditText(QString)));      //�༭ѡ�п�
    connect(this->input,SIGNAL(backSpace()),this,SLOT(backSpaceText()));               //�˸��

    connect(this->msgshow,SIGNAL(mainmenu_show()),this->mainmenu,SLOT(show()));        //��ʾ���˵�

    connect(this->timer,SIGNAL(timeout()),this,SLOT(on_Btn_cancel_clicked()));         //�ص�����������
    connect(this->mainmenu,SIGNAL(menu_exit()),this,SLOT(on_Btn_cancel_clicked()));    //�ص�����������

    ui->Btn_register->hide();//add for test

}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::paintEvent(QPaintEvent *)//��ͼ�¼�������ͼƬ
{
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/pic/pic_bg/bkg.png"));
}

void LoginForm::mousePressEvent(QMouseEvent *ev)//��갴���¼�
{
    int x = ev->x();//�������x����
    int y = ev->y();//�������y����

    this->timer->start(1000*show_menu_delay);


    qDebug()<<"x = "<<x
            <<" y = "<<y;
    qDebug()<<ev->pos();//��ʾ����pos
}

void LoginForm::Edit_Clear()
{
    ui->Edit_usrName->setText("");
    ui->Edit_pwd->setText("");
    ui->Edit_usrName->setFocus();
}

void LoginForm::EditText(QString str)//�༭ѡ�п�
{
    if(ui->Edit_usrName->hasFocus() == true )   //�жϽ�������
    {
        ui->Edit_usrName->insert(str);  //׷��
    }
    else if(ui->Edit_pwd->hasFocus()==true)
    {
        ui->Edit_pwd->insert(str);
    }
}


void LoginForm::backSpaceText()    //�����˸��
{
    QString str;
    if(ui->Edit_usrName->hasFocus() == true )
    {
        str = ui->Edit_usrName->text(); //���Ȼ�õ��ı����е�����
        if(str.isEmpty() == false)
        {
            str = str.left(str.size()-1);   //�ַ���һ
            ui->Edit_usrName->setText(str); //��������
        }
    }
    else if(ui->Edit_pwd->hasFocus() == true)
    {
        str = ui->Edit_pwd->text();
        if(str.isEmpty() == false)
        {
            str = str.left(str.size()-1);
            ui->Edit_pwd->setText(str);
        }
    }
}

void LoginForm::on_Btn_login_clicked()  //������¼
{
    int ret = 1;//normal_login(ui->Edit_usrName->text().toAscii().data(),
                           //ui->Edit_pwd->text().toAscii().data());
    if(ret == 1)
    {
        this->timer->stop();
        this->hide();
        //this->mainmenu->setGeometry(this->geometry());
        //this->mainmenu->show();
        this->msgshow->MsgShow();
    }
    else if(ret == 0)
    {
        QMessageBox::warning(this,"Warning","UserName or Password Error!");
    }
    else
    {
        qDebug()<<"the ret is "<<ret;
        qDebug()<<"program worng or db is not exits!!";
    }
}

void LoginForm::on_Btn_register_clicked()   //ע���ʺ�
{
    int ret = 1;//account_register(ui->Edit_usrName->text().toAscii().data(),
                               //ui->Edit_pwd->text().toAscii().data());
    if(ret == 1)
    {
        QMessageBox::warning(this,"congratulation","register successful!!");
    }
    else if(ret == 0)
    {
        QMessageBox::warning(this,"Warning","UserName Exist!!!");
    }
    else
    {
        qDebug()<<"the ret is "<<ret;
        qDebug()<<"program worng or db is not exits!!";
    }
}

void LoginForm::on_Btn_cancel_clicked() //�˳���¼
{
    emit bg_show();     //����ͼƬ�ֲ��ź�
    this->close();      //�رձ�����
}

void LoginForm::on_checkBox_showPwd_clicked(bool checked)   //��ʾ����
{
    if(checked)
    {
        ui->Edit_pwd->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui->Edit_pwd->setEchoMode(QLineEdit::Password);
    }
}
