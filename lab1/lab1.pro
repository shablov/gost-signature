#-------------------------------------------------
#
# Project created by QtCreator 2014-09-13T10:00:07
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = bin/app
CONFIG += console
CONFIG += debug

#TARGET = lib/cryptogenerator
#CONFIG += staticlib
#TEMPLATE = lib

OBJECTS_DIR = obj
MOC_DIR = obj

SOURCES += \
	main.cpp \
	cryptogeneratorfactory.cpp \
	cryptogeneratorinterface.cpp \
	cryptogeneratorgost_1994.cpp \
	cryptogeneratorgost_2012.cpp

HEADERS += \
	include/cryptogeneratorfactory.h \
	include/cryptogeneratorinterface.h \
	include/cryptogeneratorgost_1994.h \
	include/cryptogeneratorgost_2012.h

LIBS += -L$$PWD/../arageli/lib/ -larageli \
	-L$$PWD/../extra/ -lextra


INCLUDEPATH += $$PWD/../arageli/src/arageli \
	       $$PWD/include \
	       $$PWD/../extra
