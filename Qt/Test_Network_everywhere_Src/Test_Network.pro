#-------------------------------------------------
# Project created by MaEnlong
#-------------------------------------------------

QT += core gui
QT += network

TARGET    = Test_Network
TEMPLATE  = app
CONFIG   += release

SOURCES  += main.cpp\
            widget.cpp \
            t9input/t9widget.cpp

HEADERS  += widget.h \
            t9input/t9widget.h
FORMS    += widget.ui \
         t9input/t9widget.ui
