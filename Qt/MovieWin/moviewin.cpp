#include "moviewin.h"
#include "ui_moviewin.h"



moviewin::moviewin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::moviewin)
{
    ui->setupUi(this);

    isPlay = true;
    isSound = true;
    isStop = false;

    //去除边框
    setWindowFlags(Qt::FramelessWindowHint);
    //this->showFullScreen();

   // this->fileName = "./media_sources/future_life.flv";
    this->fileName="./future_life.flv";
    this->process = new QProcess;

    //process->start("mplayer -slave -quiet mad future_life.flv");
/*
    QString common = "mplayer -idle -slave -quiet -zoom  future_life.flv -wid " + QString::number(ui->widget_play->winId());

    qDebug()<<"this command is "<<common;

    process->start(common);     //开始运行程序
*/

}


moviewin::~moviewin()
{
    delete ui;
}

void moviewin::on_Bt_open_clicked()//打开按钮
{
    /*
    this->fileName = QFileDialog::getOpenFileName(this, tr("Open a Movie"),"");
    if (!fileName.isEmpty())
    {
        qDebug()<<"fileName="<<fileName;
        
        process->write("quit\n");
		process->close();
        QString common = "mplayer -slave -quiet  -zoom " + fileName +
                         " -wid " + QString::number(ui->widget_play->winId());
        qDebug()<<"this command is "<<common;
        process->start(common);
        
        isPlay = true;
        QIcon icon_play;
        icon_play.addPixmap(QPixmap(QString::fromUtf8(":/bt_pic/image/control_pause.png")), QIcon::Normal, QIcon::Off);
        ui->Bt_PlayPause->setIcon(icon_play);
        isStop = false;
    }
    else
    {
        this->fileName="./future_life.flv";
        qDebug()<<"filepath is empty!!! ";
    }
    */
}

void moviewin::on_Bt_stop_clicked()//停止播放按钮
{
/*
    if(!isStop)
    {
        process->write("quit\n");
		process->close();
        isPlay = false;
        QIcon icon_pause;
        icon_pause.addPixmap(QPixmap(QString::fromUtf8(":/bt_pic/image/control_play.png")), QIcon::Normal, QIcon::Off);
        ui->Bt_PlayPause->setIcon(icon_pause);
        isStop = true;
    }
    else
    {
        qDebug()<<"it is already stop !!!";
    }
*/
}

void moviewin::on_Bt_backward_clicked()//后退
{
    /*
    if(process && process->state() == QProcess::Running )
    {
        process->write("seek + 5\n");
    }
    else
    {
        qDebug()<<"process is not running!!!";
    }
    */
}

void moviewin::on_Bt_PlayPause_clicked()//暂停、播放按钮
{
    /*
    if(!isPlay)
    {
        if(isStop)
        {
			qDebug()<<"0000001";   
            QString common = "mplayer -slave -quiet -zoom " + fileName +
                             " -wid " + QString::number(ui->widget_play->winId());
			process->close();
            process->start(common);
            
            isStop = false;
        }
        else
        {
			qDebug()<<"0000002";   
            process->write("pause\n");
        }

        QIcon icon_play;
        icon_play.addPixmap(QPixmap(QString::fromUtf8(":/bt_pic/image/control_pause.png")), QIcon::Normal, QIcon::Off);
        ui->Bt_PlayPause->setIcon(icon_play);
        isPlay = true;
    }
    else
    {
		qDebug()<<"0000003";   
        QIcon icon_pause;
        icon_pause.addPixmap(QPixmap(QString::fromUtf8(":/bt_pic/image/control_play.png")), QIcon::Normal, QIcon::Off);
        ui->Bt_PlayPause->setIcon(icon_pause);
        isPlay = false;
        process->write("pause\n");
    }
    */
}

void moviewin::on_Bt_forward_clicked()//前进按钮
{
    /*
    if(process && process->state() == QProcess::Running )
    {
        process->write("seek - 5\n");
    }
    else
    {
        qDebug()<<"process is not running!!!";
    }
    */
}

void moviewin::on_HS_volume_valueChanged(int value)
{
    /*
    qDebug()<<"the vloume is "<<value;
    process->write(QString("volume +" + QString::number(value) + "\n").toAscii());
    */
}

void moviewin::on_Bt_return_clicked()
{
    on_Bt_stop_clicked();
    emit this->show_main_menu();
    this->close();
}
