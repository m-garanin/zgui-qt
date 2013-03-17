#-------------------------------------------------
#
# Project created by QtCreator 2013-03-07T12:38:26
#
#-------------------------------------------------
QT       += core gui

TARGET = zcore-stub
TEMPLATE = lib
CONFIG += staticlib

DESTDIR = $$PWD

SOURCES += zcorestub.cpp \
    manager.cpp

HEADERS += zcorestub.h \
    manager.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
