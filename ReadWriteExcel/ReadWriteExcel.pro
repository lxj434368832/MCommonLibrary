#-------------------------------------------------
#
# Project created by QtCreator 2019-03-21T10:40:01
#
#-------------------------------------------------
include(../configuration.pri)

QT       += core gui axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR     = $$IDE_APP_PATH
TARGET = $$qtLibraryTarget(ReadWriteExcel)
TEMPLATE = app
#TEMPLATE = lib
#CONFIG += staticlib

SOURCES += main.cpp\
    ReadExcel.cpp \
    TestReadWriteExcel.cpp

HEADERS  += \
    ReadExcel.h \
    TestReadWriteExcel.h

FORMS    += ReadWriteExcel.ui
