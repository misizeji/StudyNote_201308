#include "msgleavewin.h"
#include "ui_msgleavewin.h"


#include <QPainter>
#include <QMessageBox>

extern "C"  //�Ǵ�д�� C
{
    //#include "msgsnd.h"
}

MsgLeaveWin::MsgLeaveWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MsgLeaveWin)
{
    ui->setupUi(this);

    //ȥ���߿�
    setWindowFlags(Qt::FramelessWindowHint);
    //this->showFullScreen();

    //��Ӽ���
    this->input = new T9Widget(ui->Ed_LeaveMsg,this);
    this->input->move(450,30);
    this->input->show();


    //������ϵ �ź����
    connect(this->input,SIGNAL(newString(QString)),this,SLOT(EditText(QString)));      //�༭ѡ�п�
    connect(this->input,SIGNAL(backSpace()),this,SLOT(backSpaceText()));               //�˸��

}

MsgLeaveWin::~MsgLeaveWin()
{
    delete ui;
}

void MsgLeaveWin::paintEvent(QPaintEvent *)//��ͼ�¼�������ͼƬ
{
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/pic/pic_bg/bkg.png"));
}

void MsgLeaveWin::EditText(QString str)//�༭ѡ�п�
{
    if(ui->Ed_LeaveMsg->hasFocus() == true )   //�жϽ�������
    {
        ui->Ed_LeaveMsg->insertPlainText(str);  //׷��
    }
}


void MsgLeaveWin::backSpaceText()    //�����˸��
{
    QString str;
    if(ui->Ed_LeaveMsg->hasFocus() == true )
    {
        str = ui->Ed_LeaveMsg->toPlainText(); //���Ȼ�õ��ı����е�����
        if(str.isEmpty() == false)
        {
            str = str.left(str.size()-1);   //�ַ���һ
            ui->Ed_LeaveMsg->setText(str); //��������
        }
    }
}

void MsgLeaveWin::on_Bt_Exit_clicked()
{
    emit this->show_main_menu();
    this->close();
}

void MsgLeaveWin::on_Bt_LeaveMsg_clicked()
{
    int ret =0; //msg_send(ui->Ed_LeaveMsg->toPlainText().toAscii().data());
    ui->Ed_LeaveMsg->setText("");

    if(ret == 0)
    {
        QMessageBox::warning(this,"congratulation"," successful!!");
    }
    else
    {
        QMessageBox::warning(this,"congratulation"," failed!!");
    }
}
