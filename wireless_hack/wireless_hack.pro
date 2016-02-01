#-------------------------------------------------
#
# Project created by QtCreator 2016-02-01T23:27:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wireless_hack
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    scanner.cpp

HEADERS  += mainwindow.h \
    scanner.h

FORMS    += mainwindow.ui

LIBS += -lpcap
