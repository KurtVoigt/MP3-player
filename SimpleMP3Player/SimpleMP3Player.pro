#-------------------------------------------------
#
# Project created by QtCreator 2020-12-21T11:40:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimpleMP3Player
TEMPLATE = app
LIBS += -lIrrKlang -lpthread
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES     = images.qrc
