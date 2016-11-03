#ifndef MSGSHOWWIN_H
#define MSGSHOWWIN_H

#include <QWidget>

namespace Ui {
    class MsgShowWin;
}

class MsgShowWin : public QWidget
{
    Q_OBJECT

public:
    explicit MsgShowWin(QWidget *parent = 0);
    ~MsgShowWin();

private:
    Ui::MsgShowWin *ui;
};

#endif // MSGSHOWWIN_H
