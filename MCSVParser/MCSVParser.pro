#-------------------------------------------------
#
# Project created by QtCreator 2016-12-02T17:03:45
#
#-------------------------------------------------
include(../configuration.pri)

QT       -= gui

TARGET = $$qtLibraryTarget(MCSVParser)

TEMPLATE = lib
LIBS     += -L$$IDE_APP_PATH
DESTDIR  = $$IDE_APP_PATH

DEFINES += MCSVPARSER_LIBRARY

SOURCES += MCSVParser.cpp

HEADERS += MCSVParser.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
