#-------------------------------------------------
#
# Project created by QtCreator 2018-09-20T11:27:04
#
#-------------------------------------------------

include(../configuration.pri)
include(../MFramelessWidget/MFramelessWidget.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
DESTDIR     = $$IDE_APP_PATH
TARGET = MFramelessWindow


SOURCES += main.cpp\
        MFramelessWindow.cpp \
        MMainWindow.cpp \
        WidgetMain.cpp

HEADERS  += MFramelessWindow.h \
    MMainWindow.h \
    WidgetMain.h

FORMS    += MFramelessWindow.ui \
    MMainWindow.ui \
    WidgetMain.ui

RESOURCES += \
    Resource.qrc
