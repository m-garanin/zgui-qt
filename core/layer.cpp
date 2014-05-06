#include "layer.h"

Layer::Layer(QObject* src, QObject *parent) :
    QObject(parent)
{
    m_src = src;
    connect(m_src, SIGNAL(yieldFrame(const QImage&)), this, SLOT(onSourceFrame(const QImage&)));
}

void Layer::onSourceFrame(const QImage& f)
{
    yieldFrame(f);
}
