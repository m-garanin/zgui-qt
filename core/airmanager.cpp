#include <QDebug>

#include "airmanager.h"

void makeStartPacket(char** pp_buf, int* p_size, int channel_id, char* pwd, char tarif, char quality, bool acc_mode)
{
    char typ = 7;
    int size = 1 + 5 + 4 + 1 + 1 + 1 + strlen(pwd);// 13 + len(pwd)
    char* h = new char[4 + size]; //
    h[0] = size >> 24;
    h[1] = size >> 16;
    h[2] = size >> 8;
    h[3] = size >> 0;

    h[4] = typ;
    memcpy(&h[5], "START", 5);
    memcpy(&h[10], &channel_id, 4);
    h[14] = quality;
    h[15] = tarif;
    if(acc_mode)
       h[16] = 1;
    else
       h[16] = 0;

    memcpy(&h[17], pwd, strlen(pwd));

    *pp_buf = h;
    *p_size = size + 4;
}



AIRManager::AIRManager(QObject *parent) :
    QObject(parent)
{
    m_acount = 0;
}

void AIRManager::startAir(int ch_id, QString pwd, QString param_fname, QString server, QString log_fname, int width, int height, int bitrate, char tarif, char quality, int acc, int test)
{
    qDebug() << "START AIR" << width << height << param_fname;
    m_vts = m_ats = 0;

    m_server = server;

    connect(); // TODO

    m_venc = new Encoder264();

    m_venc->InitEncoder(param_fname, width, height, width, height, bitrate, "/Users/os/1/ddd.log"); // TODO: bitrate, logfile

    m_aenc = new CEncoderAAC();
    m_aenc->InitEncoder(44100,1, 128000); // TODO: bitrate
    qDebug() << "init aenc";
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
    send(m_outVideoBuf, size);
}

void AIRManager::sendAudio(uint8_t *buf, int sz, uint64 ts)
{
    int size;

    if(sz == 0)
        return;

    if(m_acount == 0){
        size = m_aenc->getInfoPacket(m_outAudioBuf);
        send(m_outAudioBuf, size);
    }


    size = m_aenc->EncodeSample(buf, sz, m_outAudioBuf);
    //qDebug() << "audio encode:" << size;
    send(m_outAudioBuf, size);

    m_acount ++;
}

bool AIRManager::connect()
{
    m_sock.connectToHost("sun.mbco.ru", 9090);
    if( !m_sock.waitForConnected())
        return false;

    char* pack;
    int size;
    makeStartPacket(&pack, &size, 8, "9SEY9", 'E', 10, 0);

    m_sock.write(pack, size);
    // TODO: wait


    return true;
}

void AIRManager::send(uint8_t *buf, int size)
{
    m_sock_mutex.lock();
    m_sock.write((char*)buf, size);
    m_sock_mutex.unlock();
}
