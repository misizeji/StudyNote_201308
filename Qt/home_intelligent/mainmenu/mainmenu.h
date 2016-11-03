#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>

#include "../ledcontrol/ledcntlwin.h"
#include "../GPRSMsg/gprsmsgwin.h"
#include "../MovieWin/moviewin.h"
#include "../MsgLeave/msgleavewin.h"
#include "../Setup/setupwin.h"
#include "../Temperature/temperaturewin.h"
#include "../usermng/usermngwin.h"

namespace Ui {
    class MainMenu;
}

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = 0);
    ~MainMenu();

private:
    Ui::MainMenu *ui;

    UserMngWin *UsrMng;         //�û�����˵�
    LedCntlWin *LedCntl;        //led�ƿ��ƴ���
    GPRSMsgWin *GprsMsg;        //GPRSģ�鷢����Ϣ
    TemperatureWin *TemCntl;    //��ʪ�ȼ�����
    MsgLeaveWin *MsgLeave;      //���Դ���
    moviewin *MovieWin;         //��Ӳ���������
    SetupWin *Setup;            //������ô���

signals:
    void menu_exit();         //�˵��˳���ť

private slots:
    void on_Bt_Setup_clicked();
    void on_Bt_Movie_clicked();
    void on_Bt_MsgLeave_clicked();
    void on_Bt_Tem_clicked();
    void on_Bt_GPRSMsg_clicked();
    void on_Bt_LedSet_clicked();
    void on_Bt_UserMng_clicked();
    void on_Bt_Exit_clicked();
    void main_menu_show();      //��ʾ���˵�����



};

#endif // MAINMENU_H
