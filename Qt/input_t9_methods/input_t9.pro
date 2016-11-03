#-------------------------------------------------
#
# Project created by QtCreator 2012-07-18T19:37:20
#
#-------------------------------------------------

QT       += core gui

TARGET = input_t9
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    t9input/t9widget.cpp \
    set_led/led.c

HEADERS  += widget.h \
    t9input/t9widget.h \
    set_led/led.h

FORMS    += widget.ui \
    t9input/t9widget.ui
