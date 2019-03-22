#-------------------------------------------------
#
# Project created by QtCreator 2018-11-26T14:45:57
#
#-------------------------------------------------

include(../configuration.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR  = $$IDE_APP_PATH
#TARGET = MComboBox
#TEMPLATE = app
TARGET = $$qtLibraryTarget(MTabWidget)
TEMPLATE = lib
CONFIG += staticlib

SOURCES += main.cpp\
        MComboBox.cpp

HEADERS  += MComboBox.h

FORMS    += MComboBox.ui
