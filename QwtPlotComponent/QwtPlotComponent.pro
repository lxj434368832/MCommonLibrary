#-------------------------------------------------
#
# Project created by QtCreator 2019-04-23T15:54:14
#
#-------------------------------------------------

include(../configuration.pri)
include(qwt.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = $$qtLibraryTarget(QwtPlotComponent)
DESTDIR  = $$PWD/lib
TEMPLATE = lib
CONFIG += staticlib

SOURCES += main.cpp \
    MainWindow.cpp \
    MPlotMagnifier.cpp \
    MCurveData.cpp \
    MSamplingThread.cpp \
    PlotBloodOxygenWave.cpp \
    PlotPulseOxygen.cpp \
    MCycleCurveData.cpp \
    MCustomAssembly.cpp \
    MPlotPanner.cpp \
    PlotCurveBase.cpp

HEADERS  += \
    MainWindow.h \
    MPlotMagnifier.h \
    MCustomAssembly.h \
    MCurveData.h \
    MSamplingThread.h \
    PlotBloodOxygenWave.h \
    PlotPulseOxygen.h \
    QwtPlotDefine.h \
    MCycleCurveData.h \
    MPlotPanner.h \
    PlotCurveBase.h

FORMS    += \
    MainWindow.ui
