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
    utils.cpp \
    effectsdlg.cpp \
    scenewidget.cpp \
    previewwidgettester.cpp

HEADERS  += mainwindow.h \
    previewwidget.h \
    lib/IManager.h \
    effectsdlg.h \
    scenewidget.h \
    previewwidgettester.h

FORMS    += mainwindow.ui \
    effectsdlg.ui
FORMS    += mainwindow.ui \
    previewwidgettester.ui

INCLUDEPATH += lib/

CONFIG += link_pkgconfig
#PKGCONFIG += gstreamer-0.10


LIBS += -L$$PWD/zcore-stub
LIBS += strmiids.lib \
        zcore-stub.lib

RESOURCES +=



