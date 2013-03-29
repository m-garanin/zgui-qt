#include "screencapturerenderer.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPixmap>
#include <QPixmap>
#include <QScreen>

ScreenCaptureRenderer::ScreenCaptureRenderer(const QRect &screenRect, QObject *parent):
    QObject(parent), m_captureRect(screenRect)
{
}

void ScreenCaptureRenderer::setSize(const QSize &size)
{
    m_captureRect.setSize(size);
}

QImage ScreenCaptureRenderer::render()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap pm = QPixmap(m_captureRect.size());
    if (screen) {
        pm = screen->grabWindow(0, m_captureRect.x(), m_captureRect.y(),
                                        m_captureRect.width(), m_captureRect.height());

    }
    return pm.toImage().convertToFormat(QImage::Format_RGB888);
}
