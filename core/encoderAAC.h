#include "faac.h"
#include "stdint.h"


class CEncoderAAC
{
public:
	CEncoderAAC(void);
	~CEncoderAAC(void);
	
    void InitEncoder(unsigned long sampleRate, unsigned int numChannels, int bitrate);

    // пакует откодированные данные в сетевой пакет(out) и возвращает его длину
    int EncodeSample(uint8_t* inBuf, int inSize, uint8_t* out);

    int getInfoPacket(uint8_t* out);

private:

	faacEncHandle m_hFAAC;
	unsigned long m_InputSamples;
	unsigned long m_MaxOutBytes;
    uint16_t m_chunk_duration; // длительность одной порции в ms (10^-3) (порция содержит m_InputSamples семплов (обычно 1024))
    uint64_t m_chunk_err;// размер ошибки округления для одной порции в ns (10^-9)(расчитывается один раз)
    uint64_t m_err_delta; // в ns (10^-9)
    uint16_t m_last_duration;

	short* m_PcmBuf;
	int m_PcmBufIndex;
	
    uint8_t* m_BitBuf; // буфер для хранения кодированных данных
	//int m_Duration;
	//int m_err_delta; // ошибки округления в ms (10^-3)
	
    int PackData(uint8_t* p_Buf, int i_size, uint16_t duration, uint8_t* out);
};
