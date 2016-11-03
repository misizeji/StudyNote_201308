#include "myplayer.h"
#include "ui_myplayer.h"
#include "phonon/VideoPlayer"

#include <QDebug>
#include <QTimer>
#include <QCloseEvent>
#include <QFileDialog>
#include <QString>

MyPlayer::MyPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyPlayer)
{
    ui->setupUi(this);
    this->player = new Phonon::VideoPlayer(Phonon::VideoCategory,ui->movieLabel);
    //player->play(Phonon::MediaSource("../blackwhite_chess/media_source/startmovie.flv"));
    this->currentMovieDirectory = "movies";
    this->timer = new QTimer(this);

    connect(ui->openButton, SIGNAL(clicked()), this, SLOT(open()));     //��
    connect(ui->playButton, SIGNAL(clicked()), this, SLOT(myplay())); //����
    connect(ui->pauseButton,SIGNAL(clicked()), this, SLOT(mypause()));   //��ͣ
    connect(ui->stopButton,SIGNAL(clicked()), this, SLOT(mystop()));   //ֹͣ
    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(myclose()));    //�ر�

    connect(this->timer, SIGNAL(timeout()), this, SLOT(updateFrameSlider()));//���½�����
    connect(ui->frameSlider, SIGNAL(valueChanged(int)), this, SLOT(goToFrame(int)));//���˿��

    this->updateFrameSlider();  //��ʼ��������
    this->setWindowTitle(tr("Movie Player"));//���ô�������
}

MyPlayer::~MyPlayer()
{
    delete ui;
    delete player;
    delete timer;
}

void MyPlayer::open()   //��֤��·��
{
    this->fileName = QFileDialog::getOpenFileName(this, tr("Open a Movie"),currentMovieDirectory);
    if (!fileName.isEmpty())
    {
        openFile(fileName);
    }
}

void MyPlayer::openFile(const QString &fileName)//���ļ�
{
    currentMovieDirectory = QFileInfo(fileName).path();
    qDebug()<<"fileName="<<fileName;
    player->play(Phonon::MediaSource(fileName));
    ui->playButton->setEnabled(false);
    ui->pauseButton->setEnabled(true);
    ui->stopButton->setEnabled(true);
    updateFrameSlider();
}

void MyPlayer::myplay()//�ҵĲ��Ű�ť���������ļ��������´�ԭ�����ļ�
{
    if(this->player->isPaused())
    {
        this->player->play();
        ui->playButton->setEnabled(false);
        ui->pauseButton->setEnabled(true);
        ui->stopButton->setEnabled(true);
    }
    else
    {
        this->player->play(Phonon::MediaSource(fileName));
        ui->pauseButton->setEnabled(true);
        ui->playButton->setEnabled(false);
        ui->stopButton->setEnabled(true);
    }
}

void MyPlayer::mypause()//�ҵĲ�������ͣ
{
    this->player->pause();
    ui->pauseButton->setEnabled(false);
    ui->playButton->setEnabled(true);
}

void MyPlayer::mystop()//�ҵĲ�����ֹͣ
{
    this->player->stop();
    ui->playButton->setEnabled(true);
    ui->pauseButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
}

void MyPlayer::goToFrame(int frame) //�������
{
    if(frame != this->player->currentTime())
    {
      this->player->seek(frame);
    }
}

void MyPlayer::updateFrameSlider()//���²��Ž�����
{
    bool hastime = (this->player->totalTime() > 0);//��ʱ�����0
    if (hastime)
    {
        if (this->player->currentTime() >= 0)
        {
            ui->frameSlider->setMaximum(this->player->totalTime() - 1);
        }
        else
        {
            if (this->player->currentTime() > ui->frameSlider->maximum())
                ui->frameSlider->setMaximum(this->player->currentTime());
        }
        ui->frameSlider->setValue(this->player->currentTime());
    }
    else
    {
        ui->frameSlider->setMaximum(0);
    }
    ui->frameLabel->setEnabled(hastime);
    ui->frameSlider->setEnabled(hastime);
    timer->start(500);//0.5��ˢ��һ��
}

void MyPlayer::myclose()//���ڹر�
{
    this->player->stop();
    this->player->close();
    this->close();
}

void MyPlayer::closeEvent(QCloseEvent *event)   //����X�ر�
{
    this->player->stop();
    this->player->close();
    this->close();
    event->accept();
}
