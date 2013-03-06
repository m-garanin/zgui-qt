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
    previewwidget.h \
    lib/IManager.h

FORMS    += mainwindow.ui

INCLUDEPATH += lib/

CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-0.10


LIBS += strmiids.lib
LIBS += C:\WORK\mbco\zgui-qt\lib\zcore.lib



