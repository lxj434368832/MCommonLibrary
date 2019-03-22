#-------------------------------------------------
#
# Project created by QtCreator 2018-12-23T17:25:53
#
#-------------------------------------------------
include(../configuration.pri)
QT       += core

TEMPLATE = lib
CONFIG += staticlib
DESTDIR  = $$IDE_APP_PATH
TARGET = $$qtLibraryTarget(MLog)

#CONFIG += console

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DEFINES += QT_MESSAGELOGCONTEXT #开启release下的logcontext

#Release:DEFINES += QT_NO_WARNING_OUTPUT \
#QT_NO_DEBUG_OUTPUT

#DEFINES += QT_NO_DEBUG_OUTPUT       #禁用qDebug输出
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    MLog.cpp

HEADERS += \
    MLog.h


CONFIG += mobility
MOBILITY =


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
