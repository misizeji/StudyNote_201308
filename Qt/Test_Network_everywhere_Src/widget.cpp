#include "widget.h"
#include "ui_widget.h"
#include <qdatetime.h>
#include <QTextCodec>
#include <QString>
#include <QFile>
#include <QDebug>

int input_status = 0;

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint);
    configFlag = false;        			//初始化连接参数 为未连接
    ui->getTextEdit->ensureCursorVisible();
    ui->sendTextEdit->setFocus();       	//程序启动时，焦点停在发送对话框
    ui->ipEdit->setText("192.168.1.230");  	//设置默认的远程端Ip
    ui->portEdit->setText("6665");          	//设置默认端口号
    this->setWindowTitle(tr("基于QT的嵌入式网络调试助手"));

    input = new T9Widget(ui->sendTextEdit,this);
    input->setGeometry(380,80,300,300);
    connect(input,SIGNAL(newString(QString)),this,SLOT(setText(QString)));
    connect(input,SIGNAL(backSpace()),this,SLOT(backSpaceText()));
    this->input->hide();

    //关联快捷键
    ui->udpSendButton->setShortcut(tr("Ctrl+s"));
    // QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
    // hostaddr1 = info.addresses().takeFirst();
    // hostaddr1 = new QHostAddress("192.168.2.214"); //本地的Ip地址
    // hostaddrStr = hostaddr1.toString();
    // hostaddrStr = QNetworkInterface::allAddresses().first().toString();

    localIpStr = getIp();
    localHostAddr = new QHostAddress(localIpStr);
    udpSocket1 = new QUdpSocket(this);

    bool bindFlag = udpSocket1->bind(*localHostAddr, 6665, QUdpSocket::ShareAddress);
    if(!bindFlag)
    {
        QMessageBox box;
        box.setText(tr("初始化绑定socket错误！"));
        box.exec();
    }
    else
    {
        connect(udpSocket1, SIGNAL(readyRead()), this, SLOT(receive()));
        connect(ui->udpSendButton, SIGNAL(clicked()), this, SLOT(send()));
    }
}

void Widget::send()
{
    autoScroll();
    QString sendStr = ui->sendTextEdit->toPlainText();
    QByteArray sendByteArray = sendStr.toAscii();
    QMessageBox box;
    if(sendStr.length()==0)
    {
        box.setText(tr("请输入发送内容"));
        box.exec();
    }
    else if(configFlag)
    {
        udpSocket1->writeDatagram(sendByteArray, sendByteArray.length(), *remoteHostAddr, 6665);
        //本地发送信息再信息交互窗口的显示
        QDateTime time;
        QString timeStr = time.currentDateTime().toString(" yyyy-MM-dd  ddd  hh:mm:ss");
        ui->getTextEdit->setTextColor(QColor("red"));
        ui->getTextEdit->insertPlainText("本地IP  " + localIpStr + "      发送时间:" + timeStr + "\n发送内容：");
        ui->getTextEdit->setTextColor(QColor("blue"));
        ui->getTextEdit->insertPlainText( sendStr +"\n\n" );
        ui->sendTextEdit->clear();          //点击发送后，发送编辑框内清零
        ui->sendTextEdit->setFocus();       //焦点停留在发送编辑框
    }
    else if(!configFlag)
    {
        box.setText("请您先点击确认按钮!");
        box.exec();
    }
    autoScroll();
}

void Widget::receive()
{
    while(udpSocket1->hasPendingDatagrams())
    {
	autoScroll();
        QTextCodec *tc=QTextCodec::codecForName("UTF-8");  //UTF-8
        QDateTime time;
        QString timeStr = time.currentDateTime().toString(" yyyy-MM-dd  ddd  hh:mm:ss");

        QByteArray data;
        data.resize(udpSocket1->pendingDatagramSize());
        udpSocket1->readDatagram(data.data(), data.size());
//      QString dataStr = QString::fromUtf8(data.data());  	//这样写也是正确的
        QString dataStr = tc->toUnicode(data);
        ui->getTextEdit->setTextColor(QColor("green"));
        ui->getTextEdit->insertPlainText("远程IP  " + remoteIpStr+"      接收时间:"+ timeStr +"\n接收内容：" );
        ui->getTextEdit->setTextColor(QColor("blue"));
        ui->getTextEdit->insertPlainText( dataStr  + "\n\n" );
        autoScroll();
    }
}

QString Widget::getIp()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)   //此处使用IPv4地址
        {
            if(address.toString().contains("169.254."))
                continue;
            if(address.toString().contains("192.16."))
                continue;
            qDebug()<<"本机Ip："<<address.toString();
            return address.toString();
        }
    }
    return 0;
}

void Widget::autoScroll()
{
    QTextCursor cursor = ui->getTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->getTextEdit->setTextCursor(cursor);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_configButton_clicked()
{
    remoteIpStr = ui->ipEdit->text();
    QString port = ui->portEdit->text();
    qDebug()<<"远程端Ip："<<remoteIpStr<<"端口号："<<port;
    remoteHostAddr = new QHostAddress(remoteIpStr);

    QMessageBox box;
    if(remoteIpStr.length()==0 || port.length()==0 || port.toInt()<1024)
    {
        configFlag = false;
        box.setText("请正确设置远程端IP地址和端口号！");
        box.exec();
    }
    else
    {
        configFlag = true;
        box.setText("您设置的远程端IP：" + remoteIpStr+" 端口号："+port);
        box.exec();
    }
}

void Widget::on_clearButton_clicked()
{
    ui->getTextEdit->clear();
}

int Widget::write_msg_in_temp(const char *s)
{
    char buf[1024];
    strcpy(buf, s);
    return system(strcat(buf, " > temp"));
}

int Widget::read_msg_in_temp(const QString& fpath)
{
    if(fpath.isEmpty())
    {
        return 1;
    }

    QFile file(fpath);

    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"Open file failure!";
        return 2;
    }

    QTextStream fs(&file);
    QString fileContent (fs.readAll());
    ui->getTextEdit->insertPlainText(fileContent);

    //QMessageBox box;
    //box.setText(fileContent);
    //box.exec();

    return 0;
}

void Widget::on_exitButton_clicked()
{
    this->close();
}

void Widget::on_configButtion_clicked()
{
    ui->getTextEdit->clear();
    write_msg_in_temp("ipconfig");
    read_msg_in_temp("./temp");
}

void Widget::on_pingButtion_clicked()
{
    ui->getTextEdit->clear();
    QString str = "ping " + ui->ipEdit->text();
    //QString str = "ping " + ui->ipEdit->text();
    QByteArray ba = str.toLatin1();
    char *PING = ba.data();
    write_msg_in_temp(PING);
    read_msg_in_temp("./temp");
}

//输入法输入焦点追加字符串
void Widget::setText(QString str)
{
    if(ui->ipEdit->hasFocus() == true)
    {
        ui->ipEdit->insert(str);
    }
    else if(ui->portEdit->hasFocus() == true)
    {
        ui->portEdit->insert(str);
    }
    else if(ui->sendTextEdit->hasFocus()==true)
    {
        ui->sendTextEdit->moveCursor(QTextCursor::End);
        ui->sendTextEdit->insertPlainText(str);
    }
}

//输入法输入退格删除字符串
void Widget::backSpaceText()
{
    QString str;
    if(ui->ipEdit->hasFocus() == true )
    {
        str = ui->ipEdit->text();
        if(str.isEmpty() == false)
        {
            str = str.left(str.size()-1);
            ui->ipEdit->setText(str);
        }
    }
    else if(ui->portEdit->hasFocus() == true)
    {
        str = ui->portEdit->text();
        if(str.isEmpty() == false)
        {
            str = str.left(str.size()-1);
            ui->portEdit->setText(str);
        }
    }
    else if(ui->sendTextEdit->hasFocus() == true)
    {
        str = ui->sendTextEdit->toPlainText();
        if(str.isEmpty() == false)
        {
            str = str.left(str.size()-1);
            ui->sendTextEdit->setPlainText(str);
        }
        ui->sendTextEdit->moveCursor(QTextCursor::End);
    }
}

void Widget::on_T9inputButton_clicked()
{
    if( input_status == 0 )
    {
        this->input->show();
        input_status = 1;
    }
    else if ( input_status == 1 )
    {
        this->input->hide();
        input_status = 0;
    }
}
