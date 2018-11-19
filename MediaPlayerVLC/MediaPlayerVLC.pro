#-------------------------------------------------
#
# Project created by QtCreator 2018-09-27T11:14:49
#
#-------------------------------------------------
include(../configuration.pri)
include($${QT_3RDPARTYPATH}/vlc/vlc.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
DESTDIR     = $$IDE_APP_PATH
TARGET = MediaPlayerVLC


SOURCES += main.cpp\
    MediaPlayerWidget/MediaPlayerWidget.cpp \
    MediaPlayerWidget/MSlider.cpp \
    MediaPlayerWidget/WidgetVolumeSlider.cpp \
    MMediaPlayer/MVLCPlayer.cpp \
    MMediaPlayer/MVLCPlayerThread.cpp \
    MMediaPlayer/MVLCPlayerSelfRender.cpp \
    TestForm.cpp

HEADERS  += \
    MediaPlayerWidget/MediaPlayerWidget.h \
    MediaPlayerWidget/MSlider.h \
    MediaPlayerWidget/WidgetVolumeSlider.h \
    MMediaPlayer/IMediaPlayer.h \
    MMediaPlayer/MediaPlayEvent.h \
    MMediaPlayer/MVLCPlayer.h \
    MMediaPlayer/MVLCPlayerThread.h \
    MMediaPlayer/MVLCPlayerSelfRender.h \
    TestForm.h

RESOURCES += \
    MediaPlayerWidget/PlayerResource.qrc

FORMS += \
    MediaPlayerWidget/MediaPlayerWidget.ui \
    MediaPlayerWidget/WidgetVolumeSlider.ui \
    TestForm.ui
