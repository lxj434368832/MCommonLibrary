#-------------------------------------------------
#
# Project created by QtCreator 2019-03-28T13:53:00
#
#-------------------------------------------------
include(../configuration.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR     = $$IDE_APP_PATH
TARGET = TestCustomGridLayout
TEMPLATE = lib


SOURCES += main.cpp \
    FormImageViewer.cpp

HEADERS  += \
    FormImageViewer.h

FORMS    += \
    FormImageViewer.ui
