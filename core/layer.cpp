#include "layer.h"

Layer::Layer(QObject* src, QSize scene_size, QObject *parent) :
    QObject(parent)
{
    m_src = src;
    m_scene_size = scene_size;
    m_visible = false;
    m_pos = QRectF(0,0,100,300);
    connect(m_src, SIGNAL(yieldFrame(const QImage&)), this, SLOT(onSourceFrame(const QImage&)));
}

void Layer::setRelationPos(QRectF pos)
{
    m_rel_pos = pos;
    QSize sz = m_scene_size;
    m_pos = QRectF(pos.x()*sz.width(), pos.y()*sz.height(),
                   pos.width()*sz.width(), pos.height()*sz.height()
                   );
}

void Layer::onSourceFrame(const QImage& f)
{
    m_img = f;
    m_size = f.size();
    yieldFrame(f);
}
