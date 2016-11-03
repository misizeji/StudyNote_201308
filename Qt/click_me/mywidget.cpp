#include "mywidget.h"

myWidget::myWidget(QWidget *parent)
    : QWidget(parent),
	button("click me",this)
{
	la = new QLabel("hello Qt",this);
        la->move(150,100);
}

myWidget::~myWidget()
{

}
