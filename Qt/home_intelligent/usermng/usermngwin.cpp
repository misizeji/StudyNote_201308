#include "usermngwin.h"
#include "ui_usermngwin.h"

#include <QtGui>
#include <QtSql>

UserMngWin::UserMngWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserMngWin)
{
    ui->setupUi(this);

    //È¥³ý±ß¿ò
    setWindowFlags(Qt::FramelessWindowHint);

    model = new QSqlTableModel(this);
    model->setTable("usr_list");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    model->setHeaderData(0, Qt::Horizontal, tr("usr"));
    model->setHeaderData(1, Qt::Horizontal, tr("password"));

    ui->view->setModel(model);

    ui->buttonBox->addButton(ui->submitButton, QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(ui->revertButton, QDialogButtonBox::ActionRole);
    //ui->buttonBox->addButton(ui->inputButton, QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(ui->quitButton, QDialogButtonBox::RejectRole);

    connect(ui->submitButton, SIGNAL(clicked()), this, SLOT(submit()));
    //connect(ui->inputButton, SIGNAL(clicked()), this, SLOT(inputshow()));
    connect(ui->revertButton, SIGNAL(clicked()), model, SLOT(revertAll()));


    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(ui->view);
    mainLayout->addWidget(ui->buttonBox);
    setLayout(mainLayout);

    //Ìí¼Ó¼üÅÌ
    /*
    this->input = new T9Widget(this);
    this->input->move(450,30);
    this->input->hide();
    showflag = false;
    */
    //¼üÅÌÁªÏµ ÐÅºÅÓë²Û
    //connect(this->input,SIGNAL(newString(QString)),this,SLOT(EditText(QString)));      //±à¼­Ñ¡ÖÐ¿ò
    //connect(this->input,SIGNAL(backSpace()),this,SLOT(backSpaceText()));               //ÍË¸ñ¼ü

    ui->inputButton->hide();

}

UserMngWin::~UserMngWin()
{
    delete ui;
}


void UserMngWin::submit()
{
    model->database().transaction();
    if (model->submitAll())
    {
        model->database().commit();
    }
    else
    {
        model->database().rollback();
        QMessageBox::warning(this, tr("Cached Table"),
                             tr("The database reported an error: %1")
                             .arg(model->lastError().text()));
    }
}
void UserMngWin::inputshow()
{
    if(!showflag)
    {
        this->input->show();
        showflag = true;
    }
    else
    {
        this->input->hide();
        showflag = false;
    }
}

void UserMngWin::on_quitButton_clicked()
{
    emit this->show_main_menu();
    this->submit();
    this->close();
}
