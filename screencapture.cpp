#include "screencapture.h"
#include <QDebug>
#include <QScreen>
#include <QApplication>
#include <QImage>
#include <QPixmap>

ScreenCapture::ScreenCapture(QString name, QRect rect, QObject *parent):
    m_name(name),       
    m_rect(rect),
    m_pOut(NULL),
    QObject(parent)
{    
    //m_rect = widg->grab_geometry();
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
    m_timer.start(40);
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

    if(m_pOut==NULL){
        // TODO: global_manager->queryIExternalSource(m_name.toLocal8Bit().data(), &m_pOut);
    }

    m_pOut->sendFrame((char*)img.bits(), img.byteCount(), img.width(), img.height());
}

void ScreenCapture::onDeleteLayer()
{
    m_timer.stop();
}
