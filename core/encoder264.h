#ifndef ENCODER264_H
#define ENCODER264_H

#include <QLibrary>
#include <QString>

#include "stdint.h"

typedef int  (__cdecl *MBCO_ENCODER_INIT)(char* param_file_name, int width, int height, int csp , int out_width, int out_height, int bitrate, char* dump_file_name, char* log_file_name);
typedef int (__cdecl *MBCO_ENCODER_ENCODE)(uint8_t *pin, int64_t pts, uint8_t *pout, int *psize);
typedef int (__cdecl *MBCO_ENCODER_HEADERS)(uint8_t *pout, int *psize);
typedef void (__cdecl *LOG)(char*, ...);

class Encoder264
{
public:
    Encoder264();

    QLibrary m_encLib;
	
    MBCO_ENCODER_INIT mbco_encoder_init;
    MBCO_ENCODER_ENCODE mbco_encoder_encode;
    MBCO_ENCODER_HEADERS mbco_encoder_headers;

    int m_Size; // размер данных в буфере

    uint64_t m_PrevDts; // dts предыдущего фрейма

    // параметры инициализации
    QString m_param_file_name;
    int m_in_width;
    int m_in_height;
    int m_bitrate;
    bool m_is_init;
    char m_log_fname[1000]; // имя log-файла

    // размеры исходящей картинки
    int m_width;
    int m_height;
    int count;

    bool m_NeedSPS_PPS;
    char m_FakeDto;
    int m_pts_delta;

    void InitEncoder(QString param_file_name, int in_width, int in_height, int out_width, int out_height, int bitrate,  char* log_fname);

    // возвращает число байт помещённых в outBuf
    // tStart - nanoseconds
    int EncodeFrame(uint8_t* inBuf, uint64_t tStart, uint8_t* outBuf);

	LOG m_log;

};

#endif // ENCODER264_H
