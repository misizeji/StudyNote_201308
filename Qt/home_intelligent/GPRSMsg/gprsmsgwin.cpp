#include "gprsmsgwin.h"
#include "ui_gprsmsgwin.h"


#include <QPainter>

extern "C"  //�Ǵ�д�� C
{
    //#include "gprs.h"
}

GPRSMsgWin::GPRSMsgWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GPRSMsgWin)
{
    ui->setupUi(this);

    //ȥ���߿�
    setWindowFlags(Qt::FramelessWindowHint);
    //this->showFullScreen();

    //��Ӽ���
    this->input = new T9Widget(ui->Ed_TelNum,this);
    this->input->move(450,30);
    this->input->show();


    //������ϵ �ź����
    connect(this->input,SIGNAL(newString(QString)),this,SLOT(EditText(QString)));      //�༭ѡ�п�
    connect(this->input,SIGNAL(backSpace()),this,SLOT(backSpaceText()));               //�˸��

}

GPRSMsgWin::~GPRSMsgWin()
{
    delete ui;
}


void GPRSMsgWin::paintEvent(QPaintEvent *)//��ͼ�¼�������ͼƬ
{
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/pic/pic_bg/bkg.png"));
}


void GPRSMsgWin::EditText(QString str)//�༭ѡ�п�
{
    if(ui->Ed_TelNum->hasFocus() == true )   //�жϽ�������
    {
        ui->Ed_TelNum->insert(str);  //׷��
    }
    else if(ui->Ed_Msg->hasFocus()==true)
    {
        ui->Ed_Msg->insertPlainText(str);
    }
}


void GPRSMsgWin::backSpaceText()    //�����˸��
{
    QString str;
    if(ui->Ed_TelNum->hasFocus() == true )
    {
        str = ui->Ed_TelNum->text(); //���Ȼ�õ��ı����е�����
        if(str.isEmpty() == false)
        {
            str = str.left(str.size()-1);   //�ַ���һ
            ui->Ed_TelNum->setText(str); //��������
        }
    }
    else if(ui->Ed_Msg->hasFocus() == true)
    {
        str = ui->Ed_Msg->toPlainText();
        if(str.isEmpty() == false)
        {
            str = str.left(str.size()-1);
            ui->Ed_Msg->setText(str);
        }
    }
}


void GPRSMsgWin::on_Bt_Exit_clicked()//�ص�������
{
    emit this->show_main_menu();
    this->close();
}

void GPRSMsgWin::on_Bt_send_clicked()//������Ϣ��ť
{
    QString str_num = ui->Ed_TelNum->text();
    QString str_msg = ui->Ed_Msg->toPlainText();
    //gprs_deal(str_num.toLatin1().data(),str_msg.toLatin1().data());
}
