#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);    

    #ifdef Q_OS_WIN32
    QString t1 = "GST_PLUGIN_PATH=" + QCoreApplication::applicationDirPath();
    _putenv(t1.toLocal8Bit().data());

    char* gpath = getenv("GSTREAMER_SDK_ROOT_X86");
    char* path = getenv("PATH");

    QString t2 = "PATH=" + QString(path) + ";"+ QString(gpath) + "bin";
    _putenv(t2.toLocal8Bit().data());
    #endif

    MainWindow w;
    w.show();

    return a.exec();
}
