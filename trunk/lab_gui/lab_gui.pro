#-------------------------------------------------
#
# Project created by QtCreator 2014-09-30T11:18:35
#
#-------------------------------------------------

QT	 += core
QT	 += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bin/app
TEMPLATE = app

MOC_DIR = obj
OBJECTS_DIR = obj

SOURCES += \
	    main.cpp\
	    mainwindow.cpp \
	    signwidget.cpp \
	    cryptowidget.cpp \
	    cryptowidgetfactory.cpp \
	    cryptowidgetgost_1994.cpp \
	    cryptowidgetgost_2012.cpp

HEADERS  += \
	    mainwindow.h \
	    signwidget.h \
	    cryptowidget.h \
	    cryptowidgetfactory.h \
	    cryptowidgetgost_1994.h \
	    cryptowidgetgost_2012.h

LIBS += \
	-L$$PWD/../lab2/lib/ -lsignature \
	-L$$PWD/../lab1/lib/ -lcryptogenerator \
	-L$$PWD/../lab3/lib/ -lhash \
	-L$$PWD/../extra/ -lextra \
	-L$$PWD/../arageli/lib/ -larageli \

INCLUDEPATH += \
		$$PWD/../lab2/include \
		$$PWD/../lab1/include \
		$$PWD/../lab3/include \
		$$PWD/../extra/ \
		$$PWD/../arageli/src/arageli \

DEPENDPATH += \
		$$PWD/../lab2/include \
		$$PWD/../lab1/include \
		$$PWD/../lab3/include \
		$$PWD/../extra/ \
		$$PWD/../arageli/src/arageli \
