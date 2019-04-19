#-------------------------------------------------
#
# Project created by QtCreator 2018-12-10T14:11:35
#
#-------------------------------------------------
include(../configuration.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR  = $$IDE_APP_PATH
TARGET = MGridLayout
TEMPLATE = app
#CONFIG+=staticlib


SOURCES += main.cpp\
        MGridLayout.cpp \
    FormImageViewer.cpp \
    FormSingleImage.cpp

HEADERS  += MGridLayout.h \
    FormImageViewer.h \
    FormSingleImage.h

FORMS += \
    FormImageViewer.ui \
    FormSingleImage.ui
