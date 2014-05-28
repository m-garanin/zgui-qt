#include <QDebug>
#include <QImage>

#include "xmsource.h"


void xm_buffer_callback(int type, char *buffer, int w, int h, int size, void *userdata)
{
    //qDebug() << "callback";
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
    if(type == 1){
        QImage img((uchar*)buffer, w, h, QImage::Format_ARGB32);

        emit yieldFrame(img);
    }else{
        qDebug() << "AUDIO:" << size;
    }
}

