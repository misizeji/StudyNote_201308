#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "inputmethod.h"

class QLabel;
class QLineEdit;
class QDialogButtonBox;

class QLogin : public QDialog
{
    Q_OBJECT

public:
    QLogin();
    ~QLogin();

public:
    InputMethod *im;
    QLabel *managerLabel;
    QLabel *passwdLabel;

    QLineEdit *managerEdit;
    QLineEdit *passwdEdit;

    QPushButton *okButton;
    QPushButton *cancelButton;
    QDialogButtonBox *buttonBox;

signals:
    void Authorize();

private slots:
    void login();
    void cancel();
    void createImConnect(QString ObjName);

};

#endif // LOGIN_H
