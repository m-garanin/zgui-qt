#ifndef AIRMANAGER_H
#define AIRMANAGER_H

#include <QObject>
#include <QImage>
#include <QTcpSocket>
#include <QMutex>

#include "IManager.h"
#include "encoder264.h"
#include "encoderAAC.h"

class AIRManager : public QObject
{
    Q_OBJECT
public:
    explicit AIRManager(QObject *parent = 0);


    void startAir(int ch_id, QString pwd,
                          QString param_fname, QString server, QString log_fname,
                          int width, int height, int bitrate, char tarif, char quality, int acc,
                          int test);

    void stopAir();

    void sendVideo(QImage img, uint64 ts);
    void sendAudio(uint8_t* buf, int sz, uint64 ts);
signals:

public slots:

private:
   Encoder264* m_venc;
   uint8_t m_outVideoBuf[1024*1024*10]; // выделяем буфер для кодированных видео-данных

   uint64 m_vts, m_ats; // времена первых семплов (больше нуля заведомо)

   CEncoderAAC* m_aenc;
   uint8_t m_outAudioBuf[1024*1024*3]; // буфер для кодированных аудио-данных
   QString m_server;

   uint64_t m_acount;
   QTcpSocket m_sock;
   QMutex m_sock_mutex;
   bool connect();

   void send(uint8_t* buf, int size);
};

#endif // AIRMANAGER_H
