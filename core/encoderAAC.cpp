#include <stdlib.h>
#include <string.h>

#include "encoderAAC.h"

CEncoderAAC::CEncoderAAC(void)
{
	//this->m_pBuf = new BYTE[1024*1024*10];
	m_err_delta = 0;
}

CEncoderAAC::~CEncoderAAC(void)
{
    free(m_PcmBuf);
    free(m_BitBuf);
	if(m_hFAAC != NULL)		
		faacEncClose(m_hFAAC);
}


void CEncoderAAC::InitEncoder(unsigned long sampleRate, unsigned int numChannels, int bitrate)
{
	numChannels = 1;
	
	m_hFAAC = faacEncOpen(sampleRate, numChannels, &m_InputSamples, &m_MaxOutBytes);
	if(m_hFAAC == NULL)
		return;
	
	faacEncConfigurationPtr pConf;
	pConf = faacEncGetCurrentConfiguration(m_hFAAC);
	pConf->inputFormat = FAAC_INPUT_16BIT;
	pConf->aacObjectType = LOW;
	pConf->mpegVersion = MPEG4;	
	pConf->allowMidside = 1;
	pConf->outputFormat = 0;
	pConf->bitRate = bitrate; // 24000/numChannels;
	pConf->bandWidth = 0;
	//pConf->bandWidth = 22000;
	//pConf->
	int iRes = faacEncSetConfiguration(m_hFAAC, pConf);
	if(!iRes)
		int y = 0;

	// расчитываем длительность порции и погрешность
	double z = ((double)(1000000000. * m_InputSamples))/ sampleRate; // длительность в ns для порции
	m_chunk_duration =  z / 1000000 ; // 10^-9 --> 10^-3 (см. протокол); // ns -> ms
	m_chunk_err = z - m_chunk_duration*1000000; // ns
	m_err_delta = 0;
	///


    m_PcmBuf = (short*)malloc(m_InputSamples*sizeof(short));
	m_PcmBufIndex = 0;
    m_BitBuf = (uint8_t*)malloc(100*m_MaxOutBytes*sizeof(uint8_t));
		
	return;
}

int CEncoderAAC::EncodeSample(uint8_t *inBuf, int inSize, uint8_t *out)
{		
	// на вход получаем стерео, но перед кодированием делаем моно
	int samples = inSize/(sizeof(short)); // получается чётное число (поскольку стерео)
	short* pSrc = (short*)inBuf;	
	int encs = 0;
	short index = 0;
	short len;

	// заполняем pcmbuf
	int j = 0;
	while(1)
	{
		m_PcmBuf[m_PcmBufIndex] = (short)pSrc[j];
		j = j + 2; // прыгаем через один
		m_PcmBufIndex ++;

		if(m_PcmBufIndex == m_InputSamples)
		{
			// encode
			len = faacEncEncode(m_hFAAC, (int32_t*)m_PcmBuf, m_InputSamples, &m_BitBuf[index+2], m_MaxOutBytes); // два байта резервируем для длинны
			if(len > 0)
			{	
				memcpy(&m_BitBuf[index], &len, 2);
				index += 2 + len;
				encs ++;			
			}
			m_PcmBufIndex = 0;		
		}

		if(j == samples) // если допрыгал до конца
			break;
	}
	
	int result_size = 0;	

	if(index > 0){
		// encs - кол-во порций
        uint16_t duration = m_chunk_duration * encs; // суммарная длительность
		m_err_delta += m_chunk_err * encs; // общая погрешность увеличиваем на суммарную погрешность порций 		
		result_size = PackData(m_BitBuf, index, duration, out);	
	}


	return result_size;
}

int CEncoderAAC::PackData(uint8_t *p_Buf, int i_size, uint16_t duration, uint8_t *out)
{
	// подготавливаем заголовок для отсылки
	int all_size = i_size + 3; // размер payload
	char h[7]; // 4+1+2
	h[0] = all_size >> 24;
	h[1] = all_size >> 16;
	h[2] = all_size >> 8;
	h[3] = all_size >> 0;
	
	h[4] = 0; //audio
	
	// заполняем duration	
    uint16_t t = duration;
	
	//////////////////////////////////////////////////////////////
	///	корректировка с учётом погрешности округления	
	if(abs(m_err_delta) > 1*1000000) // если погрешность больше 1 ms
	{
		if(m_err_delta > 0)
		{
			t = t + 1;
			m_err_delta = m_err_delta - 1*1000000;
		}else{
			t = t - 1;
			m_err_delta = m_err_delta + 1*1000000;
		}
	}
	//////////////////////////////////////////////////////////////
	
	memcpy(&h[5], &t, 2);

	memcpy(out, h, 7); // пишем хидер
	memcpy(out+7, (char*)p_Buf, i_size); // пишем тело

	m_last_duration = t;

	return i_size + 7;


}

int CEncoderAAC::getInfoPacket(uint8_t *out)
{
    uint8_t rate = 4; //(44100) TODO ?
    int size = PackData(&rate, 1, 0, out);
    return size;
}

