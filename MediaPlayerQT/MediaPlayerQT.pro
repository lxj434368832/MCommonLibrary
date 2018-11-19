#-------------------------------------------------
#
# Project created by QtCreator 2018-09-11T14:16:45
#目前测试在Qt5.6.2+vs2013环境下才能显示图像
#-------------------------------------------------
include(../configuration.pri)

QT       += core gui multimedia multimediawidgets

qtHaveModule(opengl): QT += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
DESTDIR     = $$IDE_APP_PATH
TARGET = MediaPlayerQT


SOURCES += main.cpp \
    MediaPlayerWidget/MediaPlayerWidget.cpp \
    MediaPlayerWidget/MSlider.cpp \
    MMediaPlayer/MMediaPlayer.cpp \
    MediaPlayerWidget/WidgetVolumeSlider.cpp \
    MMediaPlayer/VideoWidget.cpp \
    MMediaPlayer/VideoWidgetSurface.cpp \
    MMediaPlayer/CustomLabelSurface.cpp

HEADERS  += \
    MediaPlayerWidget/MediaPlayerWidget.h \
    MediaPlayerWidget/MSlider.h \
    MMediaPlayer/IMediaPlayer.h \
    MMediaPlayer/MediaPlayEvent.h \
    MMediaPlayer/MMediaPlayer.h \
    MediaPlayerWidget/WidgetVolumeSlider.h \
    MMediaPlayer/VideoWidget.h \
    MMediaPlayer/VideoWidgetSurface.h \
    MMediaPlayer/CustomLabelSurface.h

FORMS    += \
    MediaPlayerWidget/MediaPlayerWidget.ui \
    MediaPlayerWidget/WidgetVolumeSlider.ui

RESOURCES += \
    MediaPlayerWidget/PlayerResource.qrc
