#include <QDebug>
#include "IManager.h"
#include "scene.h"

Scene::Scene(QObject *parent) :
    QObject(parent)
{
}

Layer *Scene::addLayer(QString type, QString sourcename, QVariant ainfo)
{
    qDebug() << type << sourcename << ainfo;
    QObject* psrc = global_manager->addSource(type, sourcename, ainfo);

    Layer* pl = new Layer(psrc);
    return pl;
}

void Scene::onBkgFrame(const QImage& f)
{
    emit yieldFrame(f);
}
