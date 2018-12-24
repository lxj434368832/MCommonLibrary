#-------------------------------------------------
#
# Project created by QtCreator 2018-12-14T15:56:45
#
#-------------------------------------------------
include(../configuration.pri)
include($${QT_3RDPARTYPATH}/vlc/vlc.pri)
include(../MediaPlayerVlC/MediaPlayerVlC.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR     = $$IDE_APP_PATH
TARGET = TestMediaPlayer
TEMPLATE = app


SOURCES += main.cpp \
    MediaPlayerWidget/MediaPlayerWidget.cpp \
    MediaPlayerWidget/MSlider.cpp \
    MediaPlayerWidget/WidgetVolumeSlider.cpp \
    TestForm.cpp \
    TakeVideoCover.cpp

HEADERS  += \
    MediaPlayerWidget/MediaPlayerWidget.h \
    MediaPlayerWidget/MSlider.h \
    MediaPlayerWidget/WidgetVolumeSlider.h \
    TestForm.h \
    TakeVideoCover.h

RESOURCES += \
    MediaPlayerWidget/PlayerResource.qrc

FORMS += \
    MediaPlayerWidget/MediaPlayerWidget.ui \
    MediaPlayerWidget/WidgetVolumeSlider.ui \
    TestForm.ui
