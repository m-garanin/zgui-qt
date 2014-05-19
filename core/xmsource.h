#ifndef XMSOURCE_H
#define XMSOURCE_H

#include <QObject>

class XMSource : public QObject
{
    Q_OBJECT
public:
    explicit XMSource(QObject *parent = 0);

signals:
    void  yieldFrame(const QImage&);

public slots:

};

#endif // XMSOURCE_H
