#include "msgshowwin.h"
#include "ui_msgshowwin.h"

MsgShowWin::MsgShowWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MsgShowWin)
{
    ui->setupUi(this);
}

MsgShowWin::~MsgShowWin()
{
    delete ui;
}
