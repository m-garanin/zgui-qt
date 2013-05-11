#-------------------------------------------------
#
# Project created by QtCreator 2013-02-28T17:30:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

#webkitwidgets
#contains(QT_CONFIG, opengl):QT += opengl

TARGET = zgui-qt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    utils.cpp \
    effectsdlg.cpp \
    audiopanel.cpp \
    volumewidget.cpp \
    startrecorddialog.cpp \
    startairdialog.cpp \
    scenepanel.cpp \
    layerconstructdlg.cpp \
    settingsmanager.cpp \
    scenewidget.cpp \
    layerwidget.cpp \
    settingsdlg.cpp \
    rectselectionwidget.cpp \
    previewwidget.cpp \
    boxwidget.cpp \
    clonedwidget.cpp \
    airwidget.cpp

HEADERS  += mainwindow.h \
    lib/IManager.h \
    effectsdlg.h \
    audiopanel.h \
    volumewidget.h \
    startrecorddialog.h \
    startairdialog.h \
    scenepanel.h \
    utils.h \
    layerconstructdlg.h \
    settingsmanager.h \
    scenewidget.h \
    settingsdlg.h \
    layerwidget.h \
    rectselectionwidget.h \
    previewwidget.h \
    boxwidget.h \
    clonedwidget.h \
    zcore.h \
    airwidget.h

FORMS    += mainwindow.ui \
    effectsdlg.ui \
    startrecorddialog.ui \
    startairdialog.ui \
    clonedwidget.ui \
    settingsdlg.ui

INCLUDEPATH += lib/

#CONFIG += link_pkgconfig
#PKGCONFIG += gstreamer-0.10

win32 {
    LIBS += -lstrmiids
}


RESOURCES += \
    icons.qrc


