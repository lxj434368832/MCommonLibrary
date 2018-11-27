#-------------------------------------------------
#
# Project created by QtCreator 2018-11-27T12:05:11
#
#-------------------------------------------------
include(../configuration.pri)

QT       += widgets

DESTDIR  = $$IDE_APP_PATH
TARGET = $$qtLibraryTarget(MUtility)
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    MUtilityUI.cpp

HEADERS += \
    MUtilityUI.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
