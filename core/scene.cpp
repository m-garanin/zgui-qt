#include <QDebug>
#include "IManager.h"
#include "scene.h"

Scene::Scene(QObject *parent) :
    QObject(parent)
{
}

Layer *Scene::addLayer(QString type, QString sourcename, QString ainfo)
{
    qDebug() << type << sourcename << ainfo;
    QObject* psrc = global_manager->addSource(type, sourcename, ainfo);

    Layer* pl = new Layer();
    connect(psrc, SIGNAL(yieldFrame(const QImage&)), pl, SLOT(onSourceFrame(const QImage&)));

    return pl;
}

void Scene::onBkgFrame(const QImage& f)
{
    emit yieldFrame(f);
}
