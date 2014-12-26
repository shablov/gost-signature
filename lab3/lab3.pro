#-------------------------------------------------
#
# Project created by QtCreator 2014-10-13T17:46:29
#
#-------------------------------------------------

QT       += core

QT       -= gui

#TARGET = bin/app
#CONFIG   += console
#CONFIG   -= app_bundle
#TEMPLATE = app

TARGET = lib/hash
CONFIG += staticlib
TEMPLATE = lib

OBJECTS_DIR = obj
MOC_DIR = obj

SOURCES += \
#    main.cpp \
    hashinterface.cpp \
    hashfactory.cpp \
    hashgost_1994.cpp \
    hashgost_2012.cpp

HEADERS += \
    include/hashinterface.h \
    include/hashfactory.h \
    include/hashgost_1994.h \
    include/hashgost_2012.h \
    include/datagost_2012.h

INCLUDEPATH += $$PWD/include

