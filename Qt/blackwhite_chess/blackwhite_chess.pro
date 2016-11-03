#-------------------------------------------------
#
# Project created by QtCreator 2013-11-01T19:29:48
#
#-------------------------------------------------

QT       += core gui
QT       += phonon
QT       += network

TARGET = blackwhite_chess
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    myplayer.cpp \
    myboard.cpp \
    udp_talk.cpp \
    mylink.cpp

HEADERS  += widget.h \
    myplayer.h \
    myboard.h \
    udp_talk.h \
    mylink.h

FORMS    += widget.ui \
    myplayer.ui \
    myboard.ui \
    udp_talk.ui \
    mylink.ui

RESOURCES += \
    blackwhite_chess_pic.qrc

RC_FILE = myapp.rc
