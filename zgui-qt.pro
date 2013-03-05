#-------------------------------------------------
#
# Project created by QtCreator 2013-02-28T17:30:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zgui-qt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    previewwidget.cpp \
    utils.cpp

HEADERS  += mainwindow.h \
    previewwidget.h

FORMS    += mainwindow.ui

INCLUDEPATH += ../zcore/

CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-0.10

LIBS +=strmiids.lib
LIBS += C:\DEV\zgui\zcore-build-Desktop_Qt_5_0_0_MSVC2010_32bit_SDK-_______\debug\zcore.lib
#LIBS += C:\WORK\mbco\zgui\zcore-build-Desktop_Qt_5_0_0_MSVC2010_32bit_SDK-_______\debug\zcore.lib

