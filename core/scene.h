#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QList>

#include "layer.h"

class Scene : public QObject
{
    Q_OBJECT
public:
    explicit Scene(QSize sz, QObject *parent = 0);
    Layer* addLayer(QString type, QString sourcename, QVariant ainfo);

    void setSize(int w, int h) {m_size = QSize(w,h);}
    void setBackground(QString fname);

    QImage makeMix();
signals:
    void  yieldFrame(const QImage&);


private:
    QSize m_size;
    QPixmap* m_pix;
    QPainter* m_pnt;
    QImage m_bkg;

    QList<Layer*> m_layers; // слои в z-порядке

    void init_painter();

};

#endif // SCENE_H
