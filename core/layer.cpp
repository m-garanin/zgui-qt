#include "layer.h"

Layer::Layer(QObject *parent) :
    QObject(parent)
{
}

void Layer::onSourceFrame(const QImage& f)
{
    yieldFrame(f);
}
