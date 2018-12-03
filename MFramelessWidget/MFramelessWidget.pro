#-------------------------------------------------
#
# Project created by QtCreator 2018-07-18T13:50:39
#
#-------------------------------------------------

include(../configuration.pri)
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR  = $$IDE_APP_PATH
TARGET = $$qtLibraryTarget(MFramelessWidget)
TEMPLATE = lib
CONFIG += staticlib

SOURCES += MMoveWidget.cpp \
    MFramelessWidget.cpp \
    MFramelessDialog.cpp \
    MFramelessPrivate.cpp

HEADERS  += \
    MMoveWidget.h \
    MFramelessWidget.h \
    MFramelessDialog.h \
    MFramelessPrivate.h

FORMS +=
