#-------------------------------------------------
#
# Project created by QtCreator 2014-10-13T12:19:04
#
#-------------------------------------------------

QT       -= gui

TARGET = extra
TEMPLATE = lib
CONFIG += staticlib

SOURCES += function.cpp

HEADERS += function.h

LIBS += -L$$PWD/../arageli/lib/ -larageli

INCLUDEPATH += $$PWD/../arageli/src/arageli
DEPENDPATH += $$PWD/../arageli/src/arageli
