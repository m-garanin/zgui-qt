#ifndef SCREENCAPTURERENDERER_H
#define SCREENCAPTURERENDERER_H

#include "irenderer.h"

#include <QObject>
#include <QRect>

class ScreenCaptureRenderer : public QObject, public IRenderer
{
    Q_OBJECT
public:
    explicit ScreenCaptureRenderer(const QRect & screenRect, QObject *parent = 0);
    virtual void setSize(const QSize&size);
    virtual QImage render();

private:
    QRect   m_captureRect;
    //int     m_screenId;
};

#endif // SCREENCAPTURERENDERER_H
