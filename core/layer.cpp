#include <QDebug>

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
    qDebug() << "RELPOS" << pos;
    m_rel_pos = pos;
    QSize sz = m_scene_size;
    m_pos = QRectF(pos.x()*sz.width(), pos.y()*sz.height(),
                   pos.width()*sz.width(), pos.height()*sz.height()
                   );
}

void Layer::setOptimalSize()
{
    float width = m_scene_size.width();
    float height = m_scene_size.height();
    float r = (float)m_size.width() / m_size.height();
    float x, y, w, h;

    // смотрим первый вариант (максимальная ширина)
    w = width;
    h = w/r;
    x = 0;
    y = (height - h) / 2;
    // если высота при этом вылезает из зоны, значит нужно брать другой вариант
    if(h > height){
        h = height;
        w = h * r;
        x = (width - w) / 2;
        y = 0;
    }

    setRelationPos(QRectF(x/width,y/height,w/width,h/height));
}

void Layer::onSourceFrame(const QImage& f)
{
    m_img = f;
    m_size = f.size();

    if(m_rel_pos.isEmpty()){
        setOptimalSize();
    }
    yieldFrame(f);
}
