#-------------------------------------------------
#
# Project created by QtCreator 2019-03-26T09:39:30
#
#-------------------------------------------------

include(../configuration.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR  = $$IDE_APP_PATH
TARGET = QPainterExample
TEMPLATE = lib
CONFIG += staticlib


SOURCES += main.cpp\
        QPainterExample.cpp \
    FormLotteryNumber.cpp

HEADERS  += QPainterExample.h \
    FormLotteryNumber.h

FORMS    += QPainterExample.ui \
    FormLotteryNumber.ui
