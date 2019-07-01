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
TEMPLATE = app
#CONFIG += staticlib

SOURCES += main.cpp \
    MainWindow.cpp \
    MPlotMagnifier.cpp \
    MCurveData.cpp \
    MSamplingThread.cpp \
    PlotBloodOxygenWave.cpp \
    PlotPulseOxygen.cpp \
    qwtPlotManage.cpp \
    MCycleCurveData.cpp \
    MCycleCurveData.cpp \
    MPlotWaveCurve.cpp

HEADERS  += \
    MainWindow.h \
    MPlotMagnifier.h \
    MCustomAssembly.h \
    MCurveData.h \
    MSamplingThread.h \
    PlotBloodOxygenWave.h \
    PlotPulseOxygen.h \
    QwtPlotDefine.h \
    qwtPlotManage.h \
    MCycleCurveData.h \
    MCycleCurveData.h \
    MPlotWaveCurve.h

FORMS    += \
    MainWindow.ui
