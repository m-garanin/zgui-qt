#-------------------------------------------------
#
# Project created by QtCreator 2013-02-28T17:30:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia webkitwidgets

TARGET = zgui-qt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    previewwidget.cpp \
    utils.cpp \
    effectsdlg.cpp \
    layerwidget.cpp \
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
    scenewidget.cpp

HEADERS  += mainwindow.h \
    previewwidget.h \
    lib/IManager.h \
    effectsdlg.h \
    layerwidget.h \
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
    scenewidget.h

FORMS    += mainwindow.ui \
    effectsdlg.ui \
    startrecorddialog.ui \
    startairdialog.ui \
    menubarwidget.ui \
    clonedwidget.ui

    menubarwidget.ui


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



