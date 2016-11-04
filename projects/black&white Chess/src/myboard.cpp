#include "myboard.h"
#include "ui_myboard.h"

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QtNetwork>
#include <QPixmap>
#include <QPainter>
#include <QCursor>
#include <QTimer>
#include <QBitmap>
#include <QPaintEvent>

MyBoard::MyBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyBoard)
{
    ui->setupUi(this);
    //��������
    this->Player_flag = 1;
    //��������ͼƬ
    this->chess_pic_my = ":/new/pic/wc.png";
    this->chess_pic_other = ":/new/pic/bc.png";
    //��ʼ����������
    //��ʼ�������ά��������
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            this->chess_put[i][j] = 0;
            this->chess_put_temp[i][j] = 0;
            this->chess_put_before[0][i][j] = this->chess_put[i][j];
        }
    }
    //�м����ӷֲ�
    chess_put[3][3] = 1;
    chess_put[3][4] = 2;
    chess_put[4][3] = 2;
    chess_put[4][4] = 1;
    //��ʼ�����ά�������������
    chess_put_before[0][3][3] = 1;
    chess_put_before[0][3][4] = 2;
    chess_put_before[0][4][3] = 2;
    chess_put_before[0][4][4] = 1;
    //��ʼ�������״
    this->setCursor(QCursor(QPixmap(chess_pic_my).scaled(80,80)));
    this->mouse_flag = 1;//�����Ӧ�¼���־λ����Ӧ��
    this->a = 0; this->b = 0;//��ʼ����ʾ����
    this->put_times = 0;//��ʼ��������������

    sender = new QUdpSocket(this);//�ͻ��˽���udp�����׽���
    receiver = new QUdpSocket(this);//����������udp�����׽���
    receiver->bind(QHostAddress::Any,7000,QUdpSocket::ShareAddress);//��6666�˿�
    connect(receiver,SIGNAL(readyRead()),this,SLOT(processPendingDatagram()));//����źŽ������ݶ�ȡ
}

MyBoard::~MyBoard()
{
    delete ui;
}

void MyBoard::game_init()//�������̳�ʼ��
{
    //��������
    this->Player_flag = 1;
    //��������ͼƬ
    this->chess_pic_my = ":/new/pic/wc.png";
    this->chess_pic_other = ":/new/pic/bc.png";
    //��ʼ����������
    //��ʼ�������ά��������
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            this->chess_put[i][j] = 0;
            this->chess_put_temp[i][j] = 0;
            this->chess_put_before[0][i][j] = this->chess_put[i][j];
        }
    }
    //�м����ӷֲ�
    chess_put[3][3] = 1;
    chess_put[3][4] = 2;
    chess_put[4][3] = 2;
    chess_put[4][4] = 1;

    //this->chess_put_temp[i][j] = 0;
    //��ʼ�����ά�������������
    chess_put_before[0][3][3] = 1;
    chess_put_before[0][3][4] = 2;
    chess_put_before[0][4][3] = 2;
    chess_put_before[0][4][4] = 1;
    //��ʼ�������״
    this->setCursor(QCursor(QPixmap(chess_pic_my).scaled(80,80)));
    this->mouse_flag = 1;//�����Ӧ�¼���־λ����Ӧ��
    this->a = 0; this->b = 0;//��ʼ����ʾ����
    this->put_times = 0;//��ʼ��������������
    this->update();//���»�ͼ
}

void MyBoard::paintEvent(QPaintEvent *)//��ͼ�¼�
{
    //������
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/new/pic/lines.png"));

    //������
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(this->chess_put_before[put_times][i][j] == 1)//�ҵ�����
            {
                QPainter q(this);
                q.drawPixmap((i*(this->width()/8)),(j*(this->height()/8)),\
                            (this->width()/8),(this->height()/8),QPixmap(chess_pic_my));
            }
            else if(this->chess_put_before[put_times][i][j] == 2)//��������
            {
                QPainter q(this);
                q.drawPixmap((i*(this->width()/8)),(j*(this->height()/8)),\
                            (this->width()/8),(this->height()/8),QPixmap(chess_pic_other));
            }
            //ֻҪ�ָ����̾ͽ���ǰ���Ҳ�ָ��������Ͳ�����δ�ָ�֮ǰ����ֻ��������壩
            this->chess_put[i][j] = this->chess_put_before[put_times][i][j];
            //���������̴����м������������
            this->chess_put_temp[i][j] = this->chess_put_before[put_times][i][j];
        }
    }
}

void MyBoard::mousePressEvent(QMouseEvent *ev)//����������������
{
    if(mouse_flag == 1) //��Ӧ����¼�
    {
        //�����������
        int x = (ev->x()/(this->width()/8));//��
        int y = (ev->y()/(this->height()/8));//��
        if((chess_put[x][y] == 1)||(chess_put[x][y] == 2))//���豾���������
        {
            return;
        }
        else//����û���Ӳ��������ǰ��
        {
            if(this->play_mode == 1)    //���˶�սģʽ
            {
                pvp_game_mode(x,y); //�������˶�ս������
            }
            else if(this->play_mode == 2)//�˻���սģʽ
            {
                pvc_game_mode(x,y); //�����˻���ս������
            }
            else if(this->play_mode == 3)//�����սģʽ
            {
                pvnet_game_mode(x,y);//���������ս����
            }
            else
            {
                return;
            }
        }
        this->update();//���»�ͼ
    }
    else//����Ӧ����¼�
    {
        return;
    }
}

int MyBoard::get_count(int role)//��ñȷ�
{
    this->count = 0;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(chess_put_before[put_times][i][j] == role)
            {
                this->count++;
            }
        }
    }
    return this->count;
}

void MyBoard::set_put_before(int puttime)//���廭���� ˢ�¼���
{
    put_times = puttime;//���ص���һ��    (puttime = put_times -1)
    //�ָ�����
    if(put_times <= 0)
    {
        put_times = 0;
        this->Player_flag = 1;
        this->setCursor(QCursor(QPixmap(chess_pic_my).scaled((this->width()/8),
                                                             (this->height()/8))));
    }
    else
    {
        player_flag_change();//����ѡ�ֱ�־
        if(play_mode == 2 && Player_flag == 2)//ģʽ���˻����ֵ�����������
        {
            pvc_game_mode(3,4);//������֪���ӵ����꣬ʹ�������¶���������(3,4)
        }
    }
    emit this->update_time_signal();//���µ���ʱ����
    emit this->update_signal();//���¼Ʒְ��ź�
    //qDebug()<<"in set put before";
    this->update();//���»�ͼ
}

void MyBoard::player_flag_change()//����ѡ�ֱ�־����
{
    //����ѡ�ֱ�־
    if(Player_flag == 1)
    {
        this->Player_flag = 2;
        //�������
        this->setCursor(QCursor(QPixmap(chess_pic_other).scaled((this->width()/8),
                                                             (this->height()/8))));
    }
    else if(Player_flag == 2)
    {
        this->Player_flag = 1;
        //�������
        this->setCursor(QCursor(QPixmap(chess_pic_my).scaled((this->width()/8),
                                                             (this->height()/8))));
    }
}

int MyBoard::my_judge(int x,int y,int gridNum, void *chess_array_addr, int currentRole)//�㷨����
{
    //�����ߵ�8������
    int dir[8][2]={{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};
    if( x < 0 || x>= gridNum || y < 0 || y >= gridNum)	// �ж������Ƿ�Ϸ�
       return 0;
    //���괫���м����
    int temp_x = x, temp_y = y;
    //��������ʼ��
    int eatNum = 0;
    typedef int (*p)[gridNum];//�Զ�������
    p chessBoard = p(chess_array_addr);	//����ת�� ����ֵ���±��� chessBoard
    //����˷������������ӣ�����;
    if(chessBoard[temp_x][temp_y] != 0)
       return 0;
    for(int i = 0 ; i <8; i++)//ѭ���ж�8������
    {
        temp_x += dir[i][0]; temp_y += dir[i][1];//׼���ж���������
        //���û�г��磬�����������ǶԷ����ӣ����г��ӵĿ��ܣ� ���ǵ�ǰ��ɫ���Ҳ�Ϊ�գ�Ϊ�Է����ӣ�
        if((temp_x < gridNum && temp_x >=0 && temp_y < gridNum && temp_y >= 0)\
            && (chessBoard[temp_x][temp_y] != currentRole)\
            && (chessBoard[temp_x][temp_y] != 0))
        {
            temp_x += dir[i][0]; temp_y += dir[i][1];//�����ж���һ��

            //ͬ���ж��Ƿ����
            while(temp_x < gridNum && temp_x >=0 && temp_y < gridNum && temp_y >= 0)
            {
                //������λ����(û���ҵ��Լ�������)
                if(chessBoard[temp_x][temp_y] == 0)
                   break;
                //�ҵ����Լ������ӣ����Գ���
                if(chessBoard[temp_x][temp_y] == currentRole)
                {
                    chessBoard[x][y] = currentRole;//�ı䵱ǰ���ӣ�x,y�������ڵ���Ϣ
                    temp_x -= dir[i][0]; temp_y -= dir[i][1];

                    while((temp_x != x )||(temp_y != y))//���ӵĹ���(�ҵ��Լ�������Ǹ����)
                    {
                        chessBoard[temp_x][temp_y] = currentRole;
                        temp_x -= dir[i][0]; temp_y -= dir[i][1];
                        eatNum ++;
                    }
                    break;
                }
                //�����Ȼ�ǶԷ����ӣ������ش˷������
                temp_x += dir[i][0]; temp_y += dir[i][1];
            }
        }
        temp_x = x; temp_y = y;	//һ�������жϽ�����׼����һ������
    }
    return eatNum;
}

bool MyBoard::iseated()//�жϳ��Ӻ���
{
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            //����м������������Ϊ�����ܳ���
            if((chess_put_temp[i][j] == 0) && (my_judge(i,j,8,&chess_put_temp,Player_flag) > 0))
            {
                this->a = i;
                this->b = j;
                this->update();//ˢ���м��ά�������
                return true;
            }
        }
    }
    this->update();//ˢ���м��ά�������
    return false;
}

void MyBoard::pvp_game_mode(int x, int y)//���˶�ս������
{
    if(iseated())//������и��������ܳԵĸ��Ӵ���
    {
        if(this->my_judge(x,y,8,&chess_put,Player_flag) > 0)//�ܳԵ�����
        {
            this->put_times++;
            //���Ӵ�������ά��������
            for(int i=0;i<8;i++)
            {
                for(int j=0;j<8;j++)
                {
                    this->chess_put_before[put_times][i][j] = chess_put[i][j];
                }
            }
            player_flag_change();//����ѡ�ֱ�־
            emit this->update_time_signal();//���µ���ʱ����
            emit this->update_signal();//���¼Ʒְ��ź�
        }
    }
    else//������и�����û���ܳԵĸ��Ӵ���
    {
        player_flag_change();//����ѡ�ֱ�־
        if(iseated())   //�ж϶Է��ܷ����
        {
            return;
        }
        else//�Է�Ҳû�����ӻ���
        {
            emit this->game_over();//��Ϸ����
        }
    }
    this->update();//���»�ͼ
}

void MyBoard::pvc_game_mode(int x, int y)//�˻���ս������
{
    if(Player_flag == 1)
    {
        pvp_game_mode(x,y);
        this->mouse_flag = 0;   //�������
        this->timer = new QTimer(this);
        this->t = 3;
        timer->start(1000);
        connect(this->timer,SIGNAL(timeout()),this,SLOT(timer_deal()));//���뵹��ʱ�������л�������
    }
    else
    {
        this->mouse_flag = 0;   //�������
        this->timer = new QTimer(this);
        this->t = 3;
        timer->start(1000);
        connect(this->timer,SIGNAL(timeout()),this,SLOT(timer_deal()));//���뵹��ʱ�������л�������
        pvp_game_mode(x,y);
    }
}

void MyBoard::link_pvnet()//�������Ӻ���
{
    this->link_win = new mylink;
    this->link_win->setWindowTitle("Playing online");
    this->link_win->show();
    connect(this->link_win,SIGNAL(get_online()),this,SLOT(game_begin()));//��ս��Ϸ��ʼ
}

void MyBoard::game_begin()//�����֪�Է��Լ��ı�־λ
{
    msg_send(9,0,Player_flag);//���ҵı�־λ�����Է�
}

void MyBoard::pvnet_game_mode(int x, int y)//�����ս����
{
    if(iseated())//������и��������ܳԵĸ��Ӵ���
    {
        if(this->my_judge(x,y,8,&chess_put,Player_flag) > 0)//�ܳԵ�����
        {
            msg_send(x,y,Player_flag);
            this->put_times++;
            //���Ӵ�������ά��������
            for(int i=0;i<8;i++)
            {
                for(int j=0;j<8;j++)
                {
                    this->chess_put_before[put_times][i][j] = chess_put[i][j];
                }
            }
            //player_flag_change();//����ѡ�ֱ�־
            mouse_flag = 0;//������� �ȴ��Է�����
            emit this->update_time_signal();//���µ���ʱ����
            emit this->update_signal();//���¼Ʒְ��ź�
        }
    }
    else//������и�����û���ܳԵĸ��Ӵ���
    {
        player_flag_change();//����ѡ�ֱ�־
        if(iseated())   //�ж϶Է��ܷ����
        {
            player_flag_change();//����ѡ�ֱ�־
            msg_send(10,0,Player_flag);
            return;
        }
        else//�Է�Ҳû�����ӻ���
        {
            emit this->game_over();//��Ϸ����
        }
    }
    this->update();//���»�ͼ

}

void MyBoard::timer_deal()//����ʱ���� ��������
{
    t--;
    if(t == 0)
    {
        timer->stop();
        //������
        if(iseated()) //�õ�������������(a,b)
        {
            this->my_judge(this->a,this->b,8,&chess_put,Player_flag);//ֱ�Ӹı����
            this->put_times++;
            //���Ӵ�������ά��������
            for(int i=0;i<8;i++)
            {
                for(int j=0;j<8;j++)
                {
                    this->chess_put_before[put_times][i][j] = chess_put[i][j];
                }
            }
            player_flag_change();//����ѡ�ֱ�־
            emit this->update_time_signal();//���µ���ʱ����
            emit this->update_signal();//���¼Ʒְ��ź�
        }
        else//������и�����û���ܳԵĸ��Ӵ���
        {
            player_flag_change();//����ѡ�ֱ�־
            if(iseated())   //�ж϶Է��ܷ����
            {
                return;
            }
            else//�Է�Ҳû�����ӻ���
            {
                emit this->game_over();//��Ϸ����
            }
        }
        this->update();//���»�ͼ
        this->mouse_flag = 1;
    }
}

void MyBoard::refresh()//�طź���
{
    if(put_times <= 0)
    {
        return;
    }
    else
    {
        this->put_times_before = put_times;//��¼��ǰ�������
        this->times_temp = 0;   //�����м�ѭ������
        this->refresh_timer = new QTimer(this);
        this->refresh_timer->start(1000);
        //���ú�������ݹ�
        connect(this->refresh_timer,SIGNAL(timeout()),this,SLOT(refresh_deal()));//���뵹��ʱ�������лط�����
    }
}

void MyBoard::refresh_deal()//�ݹ���ú����ط�����
{
    this->refresh_timer->stop();
    this->mouse_flag = 0;   //�������
    this->put_times = this->times_temp;
    emit this->update_time_signal();//���µ���ʱ����
    emit this->update_signal();//���¼Ʒְ��ź�
    this->update();
    this->times_temp++;
    if(times_temp == this->put_times_before)
    {
        this->mouse_flag = 1;   //��Ӧ���
        this->put_times = put_times_before;
        this->update();
        return;
    }
    this->refresh_timer = new QTimer(this);
    this->refresh_timer->start(1000);
    //�����ݹ�ʹ��
    connect(this->refresh_timer,SIGNAL(timeout()),this,SLOT(refresh_deal()));//���뵹��ʱ�������лط�����
}

QString MyBoard::put_tips()//��ʾ���Ӻ���
{
    iseated();
    QString tip = QString("(%1,%2)").arg(this->a).arg(this->b);
    return tip;
}

void MyBoard::msg_send(int x,int y,int flag)//���ͺ���
{
    QString send_msg = QString("%1:%2:%3").arg(x).arg(y).arg(flag);
    QByteArray datagram = send_msg.toAscii();
        sender->writeDatagram(datagram.data(),datagram.size(),
                              QHostAddress(this->link_win->get_ip()),this->link_win->get_port().toInt());
}

void MyBoard::processPendingDatagram() //����ȴ������ݰ�
{
    QByteArray datagram; //���ڴ�Ž��յ����ݱ�
    while(receiver->hasPendingDatagrams())  //ӵ�еȴ������ݱ�
    {
        datagram.resize(receiver->pendingDatagramSize());
        //��datagram�Ĵ�СΪ�ȴ���������ݱ��Ĵ�С���������ܽ��յ�����������
        receiver->readDatagram(datagram.data(),datagram.size());
        //�������ݱ��������ŵ�datagram��
    }
    //�����ݱ�������ʾ����
    //qDebug()<<datagram;
    QString str = datagram;
    QStringList recv_list = str.split(":");
    qDebug()<<recv_list[0]<<" "<<recv_list[1]<<" "<<recv_list[2]<<" ";
    QString str_x = recv_list[0];
    QString str_y = recv_list[1];
    QString str_flag = recv_list[2];

    if(str_x.toInt() == 9)//��Ϸ��ʼ��֪ͨ�˴������־λ��
    {
        if(str_flag.toInt() == 1)
        {
            this->Player_flag = 2;
            this->my_flag = 2;
            this->setCursor(QCursor(QPixmap(chess_pic_other).scaled((this->width()/8),
                                                                 (this->height()/8))));
        }
        else
        {
            this->Player_flag = 1;
            this->my_flag = 1;
            this->setCursor(QCursor(QPixmap(chess_pic_my).scaled((this->width()/8),
                                                                 (this->height()/8))));
        }
    }
    else if(str_x.toInt() == 10)//���Է��������壩
    {
        mouse_flag = 1; //��Ӧ��꼺������
    }
    else//��֪����
    {
        my_judge(str_x.toInt(),str_y.toInt(),8,&chess_put,str_flag.toInt());
        this->put_times++;
        //���Ӵ�������ά��������
        for(int i=0;i<8;i++)
        {
            for(int j=0;j<8;j++)
            {
                this->chess_put_before[put_times][i][j] = chess_put[i][j];
            }
        }
        //player_flag_change();//����ѡ�ֱ�־
        mouse_flag = 1; //��Ӧ��꼺������
        emit this->update_time_signal();//���µ���ʱ����
        emit this->update_signal();//���¼Ʒְ��ź�
    }
    this->update();
}
