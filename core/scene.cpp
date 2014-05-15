#include <QDebug>
#include "IManager.h"
#include "scene.h"

Scene::Scene(QSize sz, QObject *parent) :
    m_size(sz),
    QObject(parent)
{
    init_painter();
}

Layer *Scene::addLayer(QString type, QString sourcename, QVariant ainfo)
{
    qDebug() << type << sourcename << ainfo;
    QObject* psrc = global_manager->addSource(type, sourcename, ainfo);

    Layer* pl = new Layer(psrc,m_size);
    m_layers.append(pl);

    // TODO: ловить событие изменения z-порядка и
    // пересоритировывать m_layers

    return pl;
}

void Scene::onBkgFrame(const QImage& f)
{
    //emit yieldFrame(f);
    mix(f);
}


void Scene::init_painter()
{
    m_pix = new QPixmap(m_size);
    m_pnt = new QPainter(m_pix);

    m_pnt->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);
}

void Scene::mix(const QImage &bkg)
{
    QImage res;

    m_pnt->drawImage(0,0, bkg);
    foreach (Layer* pl, m_layers) {
        if( !pl->isVisible())
            continue;

        m_pnt->drawImage(pl->getPosition(), pl->getLastImage());
    }

    //m_pnt->drawEllipse(0,0,200,400);
    res = m_pix->toImage();

    emit yieldFrame(res);
}
