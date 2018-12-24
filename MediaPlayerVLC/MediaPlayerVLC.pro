#-------------------------------------------------
#
# Project created by QtCreator 2018-09-27T11:14:49
#
#-------------------------------------------------
include(../configuration.pri)
include($${QT_3RDPARTYPATH}/vlc/vlc.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
CONFIG += staticlib
DESTDIR  = $$IDE_APP_PATH
TARGET = $$qtLibraryTarget(MediaPlayerVLC)

SOURCES += \
    MVLCPlayer.cpp \
    MVLCPlayerThread.cpp \
    MVLCPlayerSelfRender.cpp \
    IVLCPlayer.cpp

HEADERS  += \
    IMediaPlayer.h \
    MVLCPlayer.h \
    MVLCPlayerThread.h \
    MVLCPlayerSelfRender.h \
    IVLCPlayer.h
