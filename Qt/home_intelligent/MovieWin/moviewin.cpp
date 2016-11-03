#include "moviewin.h"
#include "ui_moviewin.h"

moviewin::moviewin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::moviewin)
{
    ui->setupUi(this);

	//ȥ���߿�
    setWindowFlags(Qt::FramelessWindowHint);
    
    this->process = new QProcess;

    //���ö�ʱ��
    this->timer = new QTimer(this);
    this->timer->start(1000);
    connect(this->timer,SIGNAL(timeout()),this,SLOT(write_ask_command()));
    connect(this->process,SIGNAL(readyReadStandardOutput()),this,SLOT(get_mplayer_msg()));

}

moviewin::~moviewin()
{
    delete ui;
}

void moviewin::movie_begin()
{
	isPlay = true;
    isStop = false;
	isDoubleClick = false;
	//Ϊ���´��ܹ��ٴβ���
	cur_time = 0;
	file_length = 20;
	//����
	
	//·����������ڵ�ǰ���ɵ�Ӧ�ó���Ŀ¼
	QString common = "mplayer -idle -slave -quiet -autosync 30 -vf scale=800:480 ./media_sources/future_homelife.flv ";
    qDebug()<<"this command is "<<common;

	process->write("quit\n");
	process->close();
    process->start(common);     //��ʼ���г���
	
	this->cur_value = 0;
    process->write(QString("volume " + QString::number(cur_value) + " 1\n").toAscii());
}

void moviewin::mousePressEvent(QMouseEvent *ev)//��갴���¼�
{
    pos_x_begin = ev->x();//�������x����
    pos_y_begin = ev->y();//�������y����

    //qDebug()<<" PressEvent x = "<<pos_x_begin<<" y = "<<pos_y_begin;
    //qDebug()<<ev->pos();//��ʾ����pos
}

void moviewin::mouseReleaseEvent(QMouseEvent *ev)//���̧���¼�
{
    pos_x_end = ev->x();//�������x����
    pos_y_end = ev->y();//�������y����

    //qDebug()<<" ReleaseEvent x = "<<pos_x_end<<" y = "<<pos_y_end;
    //qDebug()<<ev->pos();//��ʾ����pos
	
	mouse_deal_slots();
}

void moviewin::mouseDoubleClickEvent(QMouseEvent *ev)//���˫���¼�
{
    //qDebug()<<" DoubleClickEvent x = "<<ev->x()<<" y = "<<ev->y();
	isDoubleClick = true;
	
	mouse_deal_slots();
}

void moviewin::mouse_deal_slots()//����¼�������
{
	int x = pos_x_end - pos_x_begin;
	int y = pos_y_end - pos_y_begin;
	bool IsForward = false;
	bool IsAddVolume = false;
	
	//qDebug()<<" Before mouse_deal_slots x = "<<x
            //<<" y = "<<y;
	if(x < 0)
	{
		IsForward = false;
		x = 0 - x;
	}
	else
	{
		IsForward = true;	
	}
	
	if(y < 0)
	{
		IsAddVolume = true;
		y = 0 - y;
	}
	else
	{
		IsAddVolume = false;
	}
	//qDebug()<<" After mouse_deal_slots x = "<<x
            //<<" y = "<<y;
			
	if(isDoubleClick)
	{
		isDoubleClick = false;
		movie_return_slots();
	}
	else
	{
		if(x < 30 && y > 30)//��������
		{
			if(y < 300)
			{
				movie_volume_set((y/30),IsAddVolume);
			}
			else
			{
				movie_volume_set(100,IsAddVolume);
			}
		}
		else if(x > 30 && y < 30)//�������
		{
			if(IsForward)
			{
				movie_forward_slots();
			}
			else
			{
				movie_backward_slots();
			}
		}
		else
		{
			movie_PlayPause_slots();
		}
	}
}

void moviewin::write_ask_command()//д��������
{
    if(isPlay)
    {
        process->write("get_time_pos\n");
        process->write("get_time_length\n");
    }
}

void moviewin::get_mplayer_msg()//��ȡMPlayer��Ϣ
{
    while(process->canReadLine())
    {
        QString message(process->readLine());
        QStringList message_list = message.split("=");
          if(message_list[0] == "ANS_TIME_POSITION")
        {
            cur_time = message_list[1].toDouble();//toInt();
            cur_time = cur_time + 1;
            QTime time = int_to_time(cur_time);
            qDebug()<<"time_pos ="<<time.toString("hh:mm:ss");
            //label->setText(time.toString("hh:mm:ss"));
            //horizontalSlider->setValue(100 * curr_time / file_length);
        }
        else if(message_list[0] == "ANS_LENGTH")
        {
            file_length = message_list[1].toDouble();//toInt();
            QTime time = int_to_time(file_length);
            qDebug()<<"file_length ="<<time.toString("hh:mm:ss");
            //label_2->setText(time.toString("hh:mm:ss"));
        }
    }
	
	if(cur_time >= file_length && (!isStop))
	{
		qDebug()<<"in the end func";
		//Ϊ���´��ܹ��ٴβ���
		cur_time = 0;
		file_length = 20;
		//����
		movie_stop_slots();
	}
}

void moviewin::movie_volume_set(int value,bool flag)//��������
{
    //qDebug()<<"the vloume is "<<value;
    if(flag)
    {
        cur_value = cur_value + value;
		if(cur_value > 100)
		{
			cur_value = 100;
		}
        process->write(QString("volume " + QString::number(cur_value) + " 1\n").toAscii());
    }
    else
    {
        cur_value = cur_value - value;
		if(cur_value < 0)
		{
			cur_value = 0;
		}
        process->write(QString("volume " + QString::number(cur_value) + " 1\n").toAscii());
    }
	qDebug()<<"the cur_vloume is "<<cur_value;
}

void moviewin::movie_forward_slots()//���
{
    if(process && process->state() == QProcess::Running )
    {
		if(cur_time > (file_length - 5))
		{
			if((cur_time + 2) < file_length)
			{
				process->write(QString("seek " + QString::number(file_length - (cur_time + 2)) + "\n").toAscii());
			}
		}
		else
		{
			process->write("seek 5\n");
		}
        isPlay = true;
		movie_volume_set(0,true);//������������
    }
    else
    {
        qDebug()<<"process is not running!!!";
    }
}

void moviewin::movie_PlayPause_slots()//��ͣ
{
    if(!isPlay)//��ǰû�ڲ���
    {
        if(isStop)
        {	
			/*
			qDebug()<<" begin the new file ";
            QString common = "mplayer -idle -slave -quiet -autosync 30 -vf scale=800:480 ./media_sources/future_homelife.flv ";
            process->close();
            process->start(common);
            isStop = false;
			*/
		}
        else
        {
            process->write("pause\n");
        }
        isPlay = true;
    }
    else//��ǰ���ڲ���
    {
        process->write("pause\n");
        isPlay = false;
    }
	
	if(isPlay)
	{
		movie_volume_set(0,true);//������������
	}
}

void moviewin::movie_backward_slots()//����
{
    if(process && process->state() == QProcess::Running )
    {
		if(cur_time < 5)
		{
			process->write(QString("seek " + QString::number(- cur_time) + "\n").toAscii());
		}
        else
		{
			process->write("seek -5\n");
		}
        isPlay = true;
		movie_volume_set(0,true);//������������
    }
    else
    {
        qDebug()<<"process is not running!!!";
    }
}

void moviewin::movie_stop_slots()//ֹͣ����
{
    if(!isStop)
    {
        process->write("quit\n");
        process->close();
        isPlay = false;
        isStop = true;
    }
    else
    {
        qDebug()<<"it is already stop !!!";
    }
}
void moviewin::movie_return_slots()//�˳�����
{
    movie_stop_slots();
    emit this->show_main_menu();
    this->close();
}

QTime moviewin::int_to_time(int second)
{
    int sec = 0, min = 0, hour = 0;
    QTime time;
    if(second < 60)
    {
        sec = second;
        min = 0;
        hour = 0;
    }
    if(second >= 60 && second < 3600)
    {
        sec = second % 60;
        min = second / 60;
        hour = 0;
    }
    if(second >= 3600)
    {
        sec = second % 60;
        min = (second / 60) % 60;
        hour = second / 3600;
    }

    time.setHMS(hour,min,sec);
    return time;
}
