#-------------------------------------------------
#
# Project created by QtCreator 2016-02-04T22:31:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wireless_hack
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    scanner.cpp \
    devicedialog.cpp

HEADERS  += widget.h \
    scanner.h \
    devicedialog.h

FORMS    += widget.ui \
    devicedialog.ui

LIBS += -lpcap
