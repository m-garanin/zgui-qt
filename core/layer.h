#ifndef LAYER_H
#define LAYER_H

#include <QObject>
#include <QSize>
#include <QRectF>
#include <QImage>

class Layer : public QObject
{
    Q_OBJECT
public:
    explicit Layer(QObject* src, QSize scene_size, QObject *parent = 0);

    QObject* src(){return m_src;}
    QSize getSize() {return m_size;}
    QImage getLastImage() {return m_img;}
    void setVisible(bool vs) { m_visible = vs; }

    bool isVisible() {return m_visible;}
    QRectF getPosition() {return m_pos;} // позиция на сцене

    void setRelationPos(QRectF pos); // устанавливает относительную позицию
signals:
    void  yieldFrame(const QImage&);

public slots:
    void onSourceFrame(const QImage&);

private:
    QObject* m_src;
    QSize   m_scene_size;
    QRectF  m_pos;
    QRectF  m_rel_pos; // относительная позиция [0,1]
    QImage  m_img;
    QSize   m_size;
    bool    m_visible;
};

#endif // LAYER_H
