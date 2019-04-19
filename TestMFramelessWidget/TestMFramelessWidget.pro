#-------------------------------------------------
#
# Project created by QtCreator 2018-07-09T10:58:22
#
#-------------------------------------------------
include(../configuration.pri)
include(../MFramelessWidget/MFramelessWidget.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR     = $$IDE_APP_PATH
TARGET = TestQDockWidget
TEMPLATE = lib

SOURCES += \
        main.cpp \
        MainWindow.cpp \
    dock_widget/dock_widget.cpp \
    cameras_widget/cameras_widget.cpp \
    cameras_widget/little_cam_wgt.cpp \
    tk_signals_widget.cpp

HEADERS += \
        MainWindow.h \
    dock_widget/dock_widget.h \
    cameras_widget/cameras_widget.h \
    cameras_widget/cameras_widget_global.h \
    cameras_widget/little_cam_wgt.h \
    tk_signals_widget.h

FORMS += \
        MainWindow.ui
