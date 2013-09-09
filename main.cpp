#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QSplashScreen>
#include <QWebSettings>

void setStyle();

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);    
    QPixmap pixmap(":splash");
    QSplashScreen splash(pixmap);
    splash.show();

    a.processEvents();
    QWebSettings::enablePersistentStorage(); // для HTML5 плагинов

    #ifdef Q_OS_WIN32
    QString t1 = "GST_PLUGIN_PATH=" + QCoreApplication::applicationDirPath();
    _putenv(t1.toLocal8Bit().data());

    char* gpath = getenv("GSTREAMER_SDK_ROOT_X86");
    char* path = getenv("PATH");

    QString t2 = "PATH=" + QString(path) + ";"+ QString(gpath) + "bin";
    _putenv(t2.toLocal8Bit().data());

    //_putenv("GST_DEBUG=3");
    #endif

    setStyle();
    MainWindow w;
    w.show();
    splash.finish(&w);
    return a.exec();
}

void setStyle(){    
    qApp->setStyleSheet(
        "MainWindow, QDialog { background-image: url(:bkg);}"
        "QSplitter::handle {background: none;}"
        "#ScenePanel {background: black;}"
        "#Preview {background-image: url(:bkg_r);}"
        "#AudioPanel {background-color: transparent;}"
        "QStatusBar {color: red; font-weight: bold;}"
        "QLabel {color: white;}"
        "#Volume {border-bottom:1px dotted gray;}"
        "QToolBar {border:none;}"
        "#LiveStatus {color: white; font-size:8px;font-weight:bold;min-width:48px;}"
                "QLCDNumber {color:white;}"
                "#LayerTitle {color:white;background: black;border:none;}"


    );

    //QString st = QString("MainWindow, QDialog { background-image: url(:bkg);}");
    //st += "MainWindow > CScenePanel { background-image: url(:scene);}";
    //st += "CLayerWidget->QWidget {border:1px solid red;}";

    //QString st = QString("MainWindow { background-color: #2f353e;}");
    //
    //qApp->setStyleSheet(st);
    //qApp->setStyleSheet(st);
    //this->setStyleSheet(st);
    //->setStyleSheet(QString("#pbMute { border-image: url(:/images/mute_off.png); background-color: transparent; max-width: 150px; max-height: 150px; margin-top: 0px; margin-left: 0px; margin-right: 0px;}"));



}
