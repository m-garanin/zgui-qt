#include <QScreen>
#include <QApplication>
#include <QPixmap>

#include "screensource.h"

ScreenSource::ScreenSource(QObject *parent) :
    QThread(parent)
{
}

void ScreenSource::init(QRect rect)
{
    m_rect = rect;
    this->start();
}

void ScreenSource::onTimer()
{
    QRect rect = m_rect;
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap pm = QPixmap(rect.size());

    if (screen) {
        pm = screen->grabWindow(0, rect.x(), rect.y(), rect.width(), rect.height());
    }

    QImage img = pm.toImage().convertToFormat(QImage::Format_ARGB32_Premultiplied);
    emit yieldFrame(img);
}
