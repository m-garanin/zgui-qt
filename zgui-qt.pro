#-------------------------------------------------
#
# Project created by QtCreator 2013-02-28T17:30:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia webkitwidgets

contains(QT_CONFIG, opengl):QT += opengl

TARGET = zgui-qt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    utils.cpp \
    effectsdlg.cpp \
    audiopanel.cpp \
    volumewidget.cpp \
    menuindicator.cpp \
    menubarwidget.cpp \
    startrecorddialog.cpp \
    startairdialog.cpp \
    scenepanel.cpp \
    layerconstructdlg.cpp \
    settingsmanager.cpp \
    clonedwidget.cpp \
    graphicsitem.cpp \
    scenewidget.cpp \
    layerwidget.cpp \
    settingsdlg.cpp
    layerwidget.cpp \
    rectselectionwidget.cpp

HEADERS  += mainwindow.h \
    lib/IManager.h \
    effectsdlg.h \
    audiopanel.h \
    volumewidget.h \
    menuindicator.h \
    menubarwidget.h \
    startrecorddialog.h \
    startairdialog.h \
    scenepanel.h \
    utils.h \
    layerconstructdlg.h \
    settingsmanager.h \
    clonedwidget.h \
    graphicsitem.h \
    scenewidget.h \
    settingsdlg.h
    layerwidget.h \
    rectselectionwidget.h

FORMS    += mainwindow.ui \
    effectsdlg.ui \
    startrecorddialog.ui \
    startairdialog.ui \
    menubarwidget.ui \
    clonedwidget.ui \
    settingsdlg.ui

INCLUDEPATH += lib/

CONFIG += link_pkgconfig
#PKGCONFIG += gstreamer-0.10


LIBS += -L$$PWD/zcore-stub
LIBS += -lzcore-stub
win32 {
    LIBS += -lstrmiids
}

RESOURCES += \
    icons.qrc



