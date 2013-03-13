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
    previewwidgettester.cpp \
    boxwidget.cpp \
    layerwidget.cpp \
    audiopanel.cpp \
    volumewidget.cpp \
    menuindicator.cpp \
    menubarwidget.cpp \
    startrecorddialog.cpp \
    startairdialog.cpp \
	layerpanel.cpp \
	scenepanel.h

HEADERS  += mainwindow.h \
    previewwidget.h \
    lib/IManager.h \
    effectsdlg.h \
    scenewidget.h \
    previewwidgettester.h \
    boxwidget.h \
    layerwidget.h \
    audiopanel.h \
    volumewidget.h \
    menuindicator.h \
    menubarwidget.h \
    startrecorddialog.h \
    startairdialog.h \
	layerpanel.h \
	scenepanel.h

FORMS    += mainwindow.ui \
    effectsdlg.ui \
	layerwidget.ui\
    startrecorddialog.ui \
    startairdialog.ui \
    previewwidgettester.ui \
    menubarwidget.ui

    menubarwidget.ui


INCLUDEPATH += lib/

CONFIG += link_pkgconfig
#PKGCONFIG += gstreamer-0.10


LIBS += -L$$PWD/zcore-stub

win32 {
    LIBS += strmiids.lib \
            zcore-stub.lib
}

RESOURCES += \
    icons.qrc



