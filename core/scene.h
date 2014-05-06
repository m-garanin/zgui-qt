#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QImage>
#include "layer.h"

class Scene : public QObject
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);
    Layer* addLayer(QString type, QString sourcename, QVariant ainfo);


signals:
    void  yieldFrame(const QImage&);

public slots:
    void  onBkgFrame(const QImage&);
};

#endif // SCENE_H
