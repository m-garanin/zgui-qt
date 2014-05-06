#ifndef LAYER_H
#define LAYER_H

#include <QObject>
#include <QImage>

class Layer : public QObject
{
    Q_OBJECT
public:
    explicit Layer(QObject *parent = 0);

signals:
    void  yieldFrame(const QImage&);

public slots:
    void onSourceFrame(const QImage&);
};

#endif // LAYER_H
