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

    connect(ui->openButton, SIGNAL(clicked()), this, SLOT(open()));     //打开
    connect(ui->playButton, SIGNAL(clicked()), this, SLOT(myplay())); //播放
    connect(ui->pauseButton,SIGNAL(clicked()), this, SLOT(mypause()));   //暂停
    connect(ui->stopButton,SIGNAL(clicked()), this, SLOT(mystop()));   //停止
    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(myclose()));    //关闭

    connect(this->timer, SIGNAL(timeout()), this, SLOT(updateFrameSlider()));//更新进度条
    connect(ui->frameSlider, SIGNAL(valueChanged(int)), this, SLOT(goToFrame(int)));//快退快进

    this->updateFrameSlider();  //初始化进度条
    this->setWindowTitle(tr("Movie Player"));//设置窗口名称
}

MyPlayer::~MyPlayer()
{
    delete ui;
    delete player;
    delete timer;
}

void MyPlayer::open()   //验证打开路径
{
    this->fileName = QFileDialog::getOpenFileName(this, tr("Open a Movie"),currentMovieDirectory);
    if (!fileName.isEmpty())
    {
        openFile(fileName);
    }
}

void MyPlayer::openFile(const QString &fileName)//打开文件
{
    currentMovieDirectory = QFileInfo(fileName).path();
    qDebug()<<"fileName="<<fileName;
    player->play(Phonon::MediaSource(fileName));
    ui->playButton->setEnabled(false);
    ui->pauseButton->setEnabled(true);
    ui->stopButton->setEnabled(true);
    updateFrameSlider();
}

void MyPlayer::myplay()//我的播放按钮，不打开新文件的条件下打开原来的文件
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

void MyPlayer::mypause()//我的播放器暂停
{
    this->player->pause();
    ui->pauseButton->setEnabled(false);
    ui->playButton->setEnabled(true);
}

void MyPlayer::mystop()//我的播放器停止
{
    this->player->stop();
    ui->playButton->setEnabled(true);
    ui->pauseButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
}

void MyPlayer::goToFrame(int frame) //快进快退
{
    if(frame != this->player->currentTime())
    {
      this->player->seek(frame);
    }
}

void MyPlayer::updateFrameSlider()//更新播放进度条
{
    bool hastime = (this->player->totalTime() > 0);//总时间大于0
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
    timer->start(500);//0.5秒刷新一次
}

void MyPlayer::myclose()//窗口关闭
{
    this->player->stop();
    this->player->close();
    this->close();
}

void MyPlayer::closeEvent(QCloseEvent *event)   //窗口X关闭
{
    this->player->stop();
    this->player->close();
    this->close();
    event->accept();
}
