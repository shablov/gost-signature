#-------------------------------------------------
#
# Project created by QtCreator 2014-10-08T20:14:39
#
#-------------------------------------------------

QT       += core

QT       -= gui

OBJECTS_DIR = obj
MOC_DIR = obj

#TARGET = bin/app
#CONFIG += console
#CONFIG += debug
#TEMPLATE = app

TARGET = lib/signature
CONFIG += staticlib
TEMPLATE = lib

SOURCES += \
#	main.cpp \
	signaturefactory.cpp \
	signatureinterface.cpp \
	signaturegost_1994.cpp \
	signaturegost_2012.cpp \
	ellipticcurvepoint.cpp

HEADERS += \
	include/signatureinterface.h \
	include/signaturefactory.h \
	include/signaturegost_1994.h \
	include/signaturegost_2012.h \
	include/ellipticcurvepoint.h

LIBS += -L$$PWD/../arageli/lib/ -larageli \
	-L$$PWD/../extra/ -lextra \
	-L$$PWD/../lab1/lib/ -lcryptogenerator \
	-L$$PWD/../lab3/lib/ -lhash

INCLUDEPATH += $$PWD/../lab1/include \
	       $$PWD/../arageli/src/arageli \
	       $$PWD/../extra \
	       $$PWD/../lab3/include \
	       $$PWD/include

DEPENDPATH += $$PWD/../lab1/include \
	      $$PWD/../arageli/src/arageli \
	      $$PWD/../extra \
	      $$PWD/../lab3/include \
	      $$PWD/include
