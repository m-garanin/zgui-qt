#include "screencapture.h"
#include <QDebug>
#include <QScreen>
#include <QApplication>
#include <QImage>
#include <QPixmap>
#include "IManager.h"

ScreenCapture::ScreenCapture(QString name, RectSelectionWidget *widg, QObject *parent):
    m_name(name),       
    QObject(parent)
{    
    m_rect = widg->grab_geometry();
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
    m_timer.start(40);

    widg->close();
    widg->deleteLater();
}

void ScreenCapture::updateFrame()
{
    //
    //qDebug() << "UPDATE FRAME";
    QRect rect = m_rect;
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap pm = QPixmap(rect.size());
    if (screen) {
        pm = screen->grabWindow(0, rect.x(), rect.y(), rect.width(), rect.height());
    }

    QImage img = pm.toImage().convertToFormat(QImage::Format_ARGB32);

    global_manager->sendExternalFrame(m_name.toLocal8Bit().data(), (char*)img.bits(), img.byteCount(), img.width(), img.height());

}
