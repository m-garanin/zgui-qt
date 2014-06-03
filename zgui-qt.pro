#-------------------------------------------------
#
# Project created by QtCreator 2013-02-28T17:30:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += webkit webkitwidgets widgets multimedia

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
    airwidget.cpp \
    captureselectdialog.cpp \
    bigairstat.cpp \
    htmlrender.cpp \
    htmlsettings.cpp \
    netsourcedlg.cpp \
    core/manager.cpp \
    core/imagesource.cpp \
    core/testsource.cpp \
    core/scene.cpp \
    core/layer.cpp \
    core/screensource.cpp \
    core/htmlsource.cpp \
    core/xmsource.cpp \
    core/airmanager.cpp \
    core/encoder264.cpp \
    core/encoderAAC.cpp

HEADERS  += mainwindow.h \
    lib/IManager.h \
    lib/XInterfaces.h \
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
    airwidget.h \
    captureselectdialog.h \
    bigairstat.h \
    htmlrender.h \
    htmlsettings.h \
    netsourcedlg.h \
    core/manager.h \
    core/imagesource.h \
    core/testsource.h \
    core/scene.h \
    core/layer.h \
    core/source.h \
    core/screensource.h \
    core/htmlsource.h \
    core/xmsource.h \
    core/airmanager.h \
    core/encoder264.h \
    lib/encoder.h \
    core/encoderAAC.h \
    faac/faac.h \
    faac/faaccfg.h

FORMS    += mainwindow.ui \
    effectsdlg.ui \
    startrecorddialog.ui \
    startairdialog.ui \
    clonedwidget.ui \
    settingsdlg.ui \
    captureselectdialog.ui \
    volumewidget.ui \
    bigairstat.ui \
    htmlsettings.ui \
    netsourcedlg.ui

INCLUDEPATH += lib/ core/ faac/

#CONFIG += link_pkgconfig
#PKGCONFIG += gstreamer-0.10

win32 {
    LIBS += -lstrmiids
}

macx{
    CONFIG += c++11
    LIBRARYPATH += faac
    LIBS += -L"/Users/os/DEV/mbco/zgui-qt/LIBS/mac" -lfaac
    QMAKE_MAC_SDK = macosx10.9
}

RESOURCES += \
    icons.qrc

OTHER_FILES += \
    README.txt


