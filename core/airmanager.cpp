#include <QDebug>

#include "airmanager.h"

AIRManager::AIRManager(QObject *parent) :
    QObject(parent)
{
}

void AIRManager::startAir(int ch_id, char *pwd, char *param_fname, char *server, char *log_fname, int width, int height, int bitrate, char tarif, char quality, int acc, int test)
{
    qDebug() << "START AIR" << width << height << param_fname;
    m_vts = m_ats = 0;

    m_venc = new Encoder264();

    m_venc->InitEncoder(param_fname, width, height, width, height, bitrate, "/Users/os/1/ddd.log"); // TODO
}

void AIRManager::stopAir()
{
    qDebug() << "STOP AIR";
}

void AIRManager::sendVideo(QImage img, uint64 ts)
{
    if(m_vts == 0){
        m_vts = ts;
    }
    ts = ts - m_vts;
    QImage z = img.convertToFormat(QImage::Format_RGB888);
    int size;
    size = m_venc->EncodeFrame(z.bits(), ts, &m_outVideoBuf[0]);

    //qDebug() << "encode :" << size;
}
