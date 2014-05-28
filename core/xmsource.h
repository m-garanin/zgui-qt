#ifndef XMSOURCE_H
#define XMSOURCE_H

#include <QObject>
#include <QByteArray>
#include <QMutex>


void xm_buffer_callback(int type, char* buffer, int w, int h, int size, void* userdata);

class XMSource : public QObject
{
    Q_OBJECT
public:
    explicit XMSource(QObject *parent = 0);

    void buffer_callback(int type, char* buffer, int w, int h, int size);

    QMutex m_amutex;
    QByteArray m_abuffer;

signals:
    void  yieldFrame(const QImage&);

public slots:

};

#endif // XMSOURCE_H
