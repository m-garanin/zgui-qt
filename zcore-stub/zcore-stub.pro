#-------------------------------------------------
#
# Project created by QtCreator 2013-03-07T12:38:26
#
#-------------------------------------------------
QT       += core gui webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += webkitwidgets widgets

TARGET = zcore-stub
TEMPLATE = lib
CONFIG += staticlib

DESTDIR = $$PWD

SOURCES += zcorestub.cpp \
    manager.cpp \
    htmlrenderer.cpp \
    screencapturerenderer.cpp

HEADERS += zcorestub.h \
    manager.h \
    htmlrenderer.h \
    irenderer.h \
    screencapturerenderer.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
