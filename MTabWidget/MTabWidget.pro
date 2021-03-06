#-------------------------------------------------
#
# Project created by QtCreator 2018-08-29T14:55:49
#
#-------------------------------------------------

include(../configuration.pri)
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR  = $$IDE_APP_PATH

#TARGET = MTabWidget
TARGET = $$qtLibraryTarget(MTabWidget)
TEMPLATE = app
#CONFIG += staticlib

SOURCES += main.cpp \
    MTabWidget.cpp \
    MTabWidgetV.cpp \
    MCustomTabStyle.cpp \
    MTabWidgetLR.cpp \
    FormTestTabWidget.cpp

HEADERS  += \
    MTabWidget.h \
    MTabWidgetV.h \
    MCustomTabStyle.h \
    MTabWidgetLR.h \
    FormTestTabWidget.h

FORMS += \
    MTabWidget.ui \
    MTabWidgetV.ui \
    MTabWidgetLR.ui \
    FormTestTabWidget.ui

