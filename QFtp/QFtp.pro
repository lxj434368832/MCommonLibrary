#-------------------------------------------------
#
# Project created by QtCreator 2016-12-02T16:08:56
#
#-------------------------------------------------
QT = core network
QT       -= gui

TARGET = $$qtLibraryTarget(QFtp)
TEMPLATE = lib
CONFIG += static
CONFIG -= shared

DEFINES += QFTP_LIBRARY

SOURCES += QFtp.cpp \
    qurlinfo.cpp

HEADERS += QFtp.h\
    qurlinfo.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
