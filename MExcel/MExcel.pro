#-------------------------------------------------
#
# Project created by QtCreator 2016-12-02T15:12:46
#
#-------------------------------------------------

QT       -= gui

QT      += axcontainer

TARGET = $$qtLibraryTarget(MExcel)
TEMPLATE = lib

DEFINES += MEXCEL_LIBRARY

SOURCES += MExcel.cpp

HEADERS += MExcel.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
