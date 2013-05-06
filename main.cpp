#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);    
    QString t = "GST_PLUGIN_PATH=" + QCoreApplication::applicationDirPath();
    putenv(t.toLocal8Bit().data());

    MainWindow w;
    w.show();

    return a.exec();
}
