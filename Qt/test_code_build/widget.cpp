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
    ui->pushButton->hide();		//����PushBotton����
    ui->pushButton->show();		//��ʾPushButton
    int w = this->width();		//��õ�ǰ���ڵĿ��
    int h = this->height();		//��õ�ǰ���ڵĸ߶�
    qDebug()<<"this W = "<<w
            <<"this H = "<<h;
    int bw = ui->pushButton->width();	//���PushBotton�Ŀ��
    int bh = ui->pushButton->height();	//���PushBotton�ĸ߶�
    qDebug()<<"this W = "<<bw
            <<"this H = "<<bh;
    ui->pushButton->resize(100,30);		//��������PushButton�Ĵ�С
    qDebug()<<"bw = "<<ui->pushButton->width()
            <<" bh = "<<ui->pushButton->height()
            <<", size = "<<ui->pushButton->size();
    this->move(100,50);		//�ƶ����ڳ��ֵĳ�ʼλ��
    qDebug()<<"this x ="<<this->x()	//��ǰ���ڵ�x����
            <<" this y = "<<this->y()//��ǰ���ڵ�y����
            <<",this pos = "<<this->pos();//��ǰ���ڵ�λ��
    ui->pushButton->setGeometry(50,20,101,30);	//����PushButton����ʼλ�����С
    qDebug()<<"button "<<ui->pushButton->geometry(); //��ȡPushBotton��λ�����С��Ϣ
    ui->pushButton->setEnabled(false);	//����PushButton���ܱ�����
    qDebug()<<"button "<<ui->pushButton->isEnabled();	//�鿴PushButton�ܲ��ܰ���

    QString str1 = "63";	//����һ���ַ���
    int n = str1.toInt();	//���ַ���ת����int��
    qDebug()<<"n = "<<n;	
	
    n = 20;
    str1 = QString::number(n,16);	//����ת�����ַ�����������ת����16����
    qDebug()<<"str1 = "<<str1;
	//�������arg�ȿ��������ֻ��������ַ���
    QString str2 = QString("aa%1 %2bb,%3").arg(str1).arg(n).arg("haha");
    qDebug()<<"str2 = "<<str2;

    str1 = "aa:bb:cc:dd:ee";
	//�ַ����������
    str2 = str1.section(":",1,1);//ȡ��һ����:��֮���һ���ַ���"bb"
    qDebug()<<"str2 = "<<str2;

	//�ַ����������
    QStringList list;	//����һ���ַ�������	
    list<<str2<<"aa"<<"bb";	//��֮ǰ�����str2:"bb",��"aa","bb"�������
    for(int i = 0;i < list.size();i++)
    {
        QString str = list[i];
        qDebug()<<i<<","<<str;
    }

    qDebug()<<"list contains aa="<<list.contains("qq");		//�ж��ַ������Ƿ����ַ���"qq"
    qDebug()<<"list contains aa="<<list.contains("aa");		//�ж��ַ������Ƿ����ַ���"aa"
	

    str2 = list.join(":");		//���ַ���������ÿ���ַ���֮�����һ��":"
    qDebug()<<"str2 = "<<str2;
    str2 = "11:22:33:44:55";
    list = str2.split(":");		//�ַ�����:�ָ�ɶ���ַ������γ��ַ�������
	//��ӡ���ַ����������ַ����ĸ��������ַ��������ӡ����
    qDebug()<<"list size="<<list.size()<<"\n"<<list;	

    ui->label->setText("hello Qt");		//���ñ�ǩ��ʾ�ַ�
    ui->label->setPixmap(QPixmap("../image/network-globe2.png"));//���ñ�ǩͼƬ
    ui->label->setScaledContents(true);		//���ñ�ǩͼƬ�Զ���Ӧ
    qDebug()<<"label text = "<<ui->label->text();	//��ȡ��ǩ����

    QMovie *movie = new QMovie(this);	//���������ÿռ�
    movie->setFileName("../image/boy.gif");	//���ö���

    ui->label_2->setMovie(movie);	//�����������ڱ�ǩ2��
    movie->start();	//��������
    ui->label_2->setScaledContents(true);	//���ö����Զ���Ӧ��С

    ui->lcdNumber->setDigitCount(4);	//������ʾ��λ��
    ui->lcdNumber->display(3.14);		//������ʾ������

    ui->progressBar->setMinimum(0);		//���ý���������Сֵ
    ui->progressBar->setMaximum(200);	//���ý����������ֵ
    ui->progressBar->setValue(100);		//���ý������ĵ�ǰֵ

    ui->lineEdit->setText("123456");	//�����б༭����
    ui->lineEdit->insert("abc");		//����"abc"���б༭��β��
    ui->lineEdit->setEchoMode(QLineEdit::Password);		//���ñ༭��ʾģʽ
    qDebug()<<ui->lineEdit->text();		//����б༭����

    ui->textEdit->setFontPointSize(10);		//���������С
    ui->textEdit->setTextColor(QColor(255,0,0));	//����������ɫ
    ui->textEdit->setText("abcde");				//�����ı��༭������
    ui->textEdit->setFontPointSize(15);			//������������Ĵ�С
    ui->textEdit->setTextColor(Qt::cyan);		//������ɫΪcyan
    ui->textEdit->append("���");				//������ʾ"���"
	qDebug()<<ui->textEdit->toPlainText();		//��ȡ�ı��༭���е�����

    ui->comboBox->setEditable(true);			//����������ɱ༭
    ui->comboBox->addItem("linux ����");		//�����Ŀ"linux����"
    ui->comboBox->addItem(QIcon("../image/myheart.png"),"Qt");//�����Ŀ+ͼ��
    ui->comboBox->insertItem(2,"C++");	//������Ŀ"C++"���ڶ�λ���ӵ���λ��ʼ����
    qDebug()<<ui->comboBox->count();	//��ȡ������Ŀ������
    ui->comboBox->setCurrentIndex(2);	//���ó�ʼ����ʾ����Ŀ�ǵ�2����Ŀ
    qDebug()<<ui->comboBox->currentText()<<", "
            <<ui->comboBox->currentIndex();		//��ȡ��ǰ����Ŀ�뵱ǰ��Ŀռ����Ŀ�е�λ��
    ui->pushButton_2->setText("yes");		//����pushbutton2��ʾ����
    ui->toolButton->setText("haha");		//����toolbutton��ʾ����
    ui->radioButton->setText("heihei");		//����radiobutton��ʾ������
    ui->checkBox->setText("����");			//��ʾ��ѡ����ʾ������
    qDebug()<<"check box = "<<ui->checkBox->text();	//��ø�ѡ����ʾ����
    ui->toolButton->setIcon(QIcon(":/new/prefix1/face-1.png"));	//����toolButton����ͼƬ
    ui->toolButton->resize(40,60);	//����toolButton�Ĵ�С
    ui->toolButton->setIconSize(QSize(ui->toolButton->width(),60));//����ͼƬ��С
    ui->toolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);//���������ڹ��߰�ťͼƬ����

    ui->radioButton->setIcon(QIcon("../image/wright.png"));//����radioButtonͼ��
    ui->radioButton->setIconSize(QSize(20,20));//����radioButton��ͼ���С

    ui->radioButton->setChecked(true);//���õ�ѡ��ѡ��״̬
    qDebug()<<ui->radioButton->isChecked();	//��ȡradioButton�Ƿ�ѡ��

	//�½�����
    QFont font;
    font.setFamily("��Բ");
    font.setPointSize(20);
    font.setBold(true);
    font.setUnderline(true);
    ui->textEdit->setFont(font);

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));	//��ȡ��ǰʱ������
    int t = qrand();
    ui->lcdNumber->setDigitCount(QString::number(t).size());//����lcd��ʾ��λ��
    ui->lcdNumber->display(t);//lcd��ʾʱ������

    QString time = QTime::currentTime().toString("hh:mm:ss");//����ǰʱ��ת�����ַ���

    ui->textEdit->setText(time);//�ı��༭����ʾʱ���ַ���
    ui->textEdit->append(QDate::currentDate().toString("yyyy-MM-dd"));//����ת���ַ���

    QPalette p;	//�½�����
    //p.setColor(QPalette::Window,Qt::green);
    p.setColor(QPalette::WindowText,Qt::magenta);	//����������ɫ��ɫ
    p.setBrush(QPalette::Window,QBrush(QPixmap(":/new/prefix1/bk.jpg")));//���ñ���ͼƬ
    ui->lcdNumber->setAutoFillBackground(true);	//�Զ���������
    ui->lcdNumber->setPalette(p);	//lcd����Palette	
}
Widget::~Widget()
{
    delete ui;
}
