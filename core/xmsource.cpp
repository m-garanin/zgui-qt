#include <QDebug>
#include <QImage>

#include "xmsource.h"

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
        // помещаем данные в очередь
        // при этом если очередь большая, то скидываем её

        //qDebug() << "AUDIO:" << size;
        m_amutex.lock();
        if(m_abuffer.length() > 10*size){
            m_abuffer.clear();
        }

        m_abuffer.append(buffer, size);

        m_amutex.unlock();
    }
}

