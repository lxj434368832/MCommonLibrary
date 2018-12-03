#-------------------------------------------------
#
# Project created by QtCreator 2016-12-02T15:09:17
#
#-------------------------------------------------

include(../configuration.pri)
#include(../MCSVParser/MCSVParser.pri)
include(../MFramelessWidget/MFramelessWidget.pri)
#include(../Mqwt/Mqwt.pri)

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE    = app
DESTDIR     = $$IDE_APP_PATH
TARGET      = $$IDE_APP_TARGET


SOURCES += main.cpp\
        Test.cpp

HEADERS  += Test.h \
    TestGlobal.h

FORMS    += Test.ui
