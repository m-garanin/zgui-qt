#include <QDebug>

#include "encoder264.h"
#include "encoder.h"
#include <string.h>
#include <stdlib.h>

#define NANO_MULT 1000000

Encoder264::Encoder264()
{
    this->count = 0;

    this->m_Size = 0;
    this->m_PrevDts = 0;
    this->m_FakeDto = 0;
    this->m_NeedSPS_PPS = true;
    m_pts_delta = 0;

    // загрузка функций из xencoder
    m_encLib.setFileName("xencoder"); // qt само поставляет dll | dylib | so

    bool r = m_encLib.load();

    if (!r) {
        qDebug() << "ERROR LOAD XENCODER LIBRARY:" << m_encLib.errorString();
    }

    mbco_encoder_init = (MBCO_ENCODER_INIT)m_encLib.resolve("mbco_encoder_init");
    mbco_encoder_encode = (MBCO_ENCODER_ENCODE)m_encLib.resolve("mbco_encoder_encode");
    mbco_encoder_headers = (MBCO_ENCODER_HEADERS)m_encLib.resolve("mbco_encoder_headers");

    if(!mbco_encoder_init){
      qDebug() << "FAILED LOAD XENCODER INIT";
    }
}

void Encoder264::InitEncoder(QString param_file_name, int in_width, int in_height, int out_width, int out_height, int bitrate, char *log_fname)
{   
    m_param_file_name = param_file_name;
    m_in_width = in_width;
    m_in_height = in_height;

    m_width = out_width;
    m_height = out_height;
    m_bitrate = bitrate;
    m_is_init = false;

    strcpy(m_log_fname, log_fname);

}

int Encoder264::EncodeFrame(uint8_t *inBuf, uint64_t tStart, uint8_t *outBuf)
{
    this->count ++;

    if(this->m_is_init == false)
    {
        int r = this->mbco_encoder_init(m_param_file_name.toLocal8Bit().data(),
                                        m_in_width, m_in_height,
                                        CSP_RGB24,
                                        m_width, m_height,
                                        m_bitrate,
                                        NULL,
                                        m_log_fname);

        this->m_is_init = true;
    }

    int result_size = 0;

    // если SPS\PPS ещё неписали, то пишем SPS & PPS
    if(m_NeedSPS_PPS)
    {
        m_NeedSPS_PPS = false;
        int r = this->mbco_encoder_headers((uint8_t*)outBuf, &result_size);

    }

    // кодирование фрейма
    int s = 0;
    uint64_t pts = double(tStart)/NANO_MULT; // получаем ms (10^-3) // TODO

    //////////////////////////////////////////////////////////////
    ///	корректировка с учётом погрешности округления
    this->m_pts_delta = this->m_pts_delta + (tStart - pts * NANO_MULT);

    //qDebug() << "PTS pure(" << this->count << ") :" << pts << "delta" << this->m_pts_delta << tStart;

    if(abs(m_pts_delta) > 1*NANO_MULT) // если погрешность больше 1 ms
    {
        if(m_pts_delta > 0)
        {
            pts = pts + 1;
            m_pts_delta = m_pts_delta - 1*NANO_MULT ;
        }else{
            pts = pts - 1;
            m_pts_delta = m_pts_delta + 1*NANO_MULT ;
        }


    }


    int r = this->mbco_encoder_encode(inBuf, pts, (uint8_t*)(outBuf + result_size), &s);
    result_size = result_size + s;

	/*
    if(pts - m_PrevDts != 40){
		m_log("WARNING DUR: %d", pts - m_PrevDts);
	}
	*/
	
    m_PrevDts = pts;

	

    //qDebug() << "PTS (" << this->count << ") :" << pts;

    return result_size;
}
