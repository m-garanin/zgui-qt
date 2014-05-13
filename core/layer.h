#ifndef LAYER_H
#define LAYER_H

#include <QObject>
#include <QSize>
#include <QImage>

class Layer : public QObject
{
    Q_OBJECT
public:
    explicit Layer(QObject* src, QObject *parent = 0);

    QObject* src(){return m_src;}
    QSize getSize() {return m_size;}
    QImage getLastImage() {return m_img;}

signals:
    void  yieldFrame(const QImage&);

public slots:
    void onSourceFrame(const QImage&);

private:
    QObject* m_src;
    QImage  m_img;
    QSize   m_size;
};

#endif // LAYER_H
