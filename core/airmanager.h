#ifndef AIRMANAGER_H
#define AIRMANAGER_H

#include <QObject>
#include <QImage>

#include "IManager.h"
#include "encoder264.h"

class AIRManager : public QObject
{
    Q_OBJECT
public:
    explicit AIRManager(QObject *parent = 0);


    void startAir(int ch_id, char* pwd,
                          char* param_fname, char* server, char* log_fname,
                          int width, int height, int bitrate, char tarif, char quality, int acc,
                          int test);

    void stopAir();

    void sendVideo(QImage img, uint64 ts);
signals:

public slots:

private:
   Encoder264* m_venc;
   uint8_t m_outVideoBuf[1024*1024*10]; // выделяем буфер для кодированных видео-данных
   uint64 m_vts, m_ats; // времена первых семплов (больше нуля заведомо)
};

#endif // AIRMANAGER_H
