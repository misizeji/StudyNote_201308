#-------------------------------------------------
#
# Project created by QtCreator 2013-11-07T08:47:09
#
#-------------------------------------------------

QT       += core gui
QT       += phonon

TARGET = chess
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    chessbox.cpp \
    myjudge.cpp \
    startmovie.cpp \
    startwindow.cpp

HEADERS  += widget.h \
    chessbox.h \
    myjudge.h \
    startmovie.h \
    startwindow.h

FORMS    += widget.ui \
    startmovie.ui \
    startwindow.ui

RESOURCES += \
    backgouand.qrc
RC_FILE = black.rc