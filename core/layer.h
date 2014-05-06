#ifndef LAYER_H
#define LAYER_H

#include <QObject>
#include <QImage>

class Layer : public QObject
{
    Q_OBJECT
public:
    explicit Layer(QObject* src, QObject *parent = 0);

    QObject* src(){return m_src;}

signals:
    void  yieldFrame(const QImage&);

public slots:
    void onSourceFrame(const QImage&);

private:
    QObject* m_src;
};

#endif // LAYER_H
