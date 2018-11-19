#-------------------------------------------------
#
# Project created by QtCreator 2017-03-24T10:53:50
#
#-------------------------------------------------
include(../configuration.pri)
include($${QT_3RDPARTYPATH}/qwt/qwtConfig.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = $$qtLibraryTarget(Mqwt)

TEMPLATE = lib

DEFINES += MQWT_LIBRARY


SOURCES += \
    MPlotMagnifier.cpp \
    MPlotPanner.cpp

HEADERS +=\
        mqwt_global.h \
    MPlotMagnifier.h \
    MPlotPanner.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
