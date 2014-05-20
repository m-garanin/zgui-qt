#include <QDebug>
#include <QImage>

#include "xmsource.h"


void xm_buffer_callback(int type, char *buffer, int w, int h, int size, void *userdata)
{
    XMSource* psrc = (XMSource*)userdata;
    psrc->buffer_callback(type, buffer, w, h, size);
}


XMSource::XMSource(QObject *parent) :
    QObject(parent)
{
}

void XMSource::buffer_callback(int type, char *buffer, int w, int h, int size)
{
    //qDebug() << "BUFFER CALLBACK";
    QImage img((uchar*)buffer, w, h, QImage::Format_ARGB32);

    emit yieldFrame(img);
}

