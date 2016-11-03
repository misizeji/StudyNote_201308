#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QtGui/QWidget>
#include <QPushButton>
#include <QLabel>

class myWidget : public QWidget
{
    Q_OBJECT
private:
	QPushButton button;
	QLabel *la;

public:
    myWidget(QWidget *parent = 0);
    ~myWidget();
};

#endif // MYWIDGET_H
