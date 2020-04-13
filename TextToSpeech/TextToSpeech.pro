#-------------------------------------------------
#
# Project created by QtCreator 2019-12-25T10:59:37
#
#-------------------------------------------------
include(../configuration.pri)

#INCLUDEPATH += C:\Program Files\Microsoft Speech SDK 5.4\Include

QT       -= gui
QT += axcontainer

DESTDIR     = $$IDE_APP_PATH
TARGET = $$qtLibraryTarget(TextToSpeech)
TEMPLATE = lib
CONFIG += staticlib
#CONFIG += console

SOURCES += TextToSpeech.cpp \
    QWinTTS.cpp

HEADERS += \
    ITTS.h \
    QWinTTS.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
