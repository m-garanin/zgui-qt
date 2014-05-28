#include <QDebug>

#include <QAudioFormat>

#include "amsource.h"

AMSource::AMSource(QObject *parent) :
    XMSource(parent)
{
    initOutput();
}

void AMSource::buffer_callback(int type, char *buffer, int w, int h, int size)
{
    //qDebug()  << "AMSource buffer_callback";
    m_output->write(buffer, size);

}


void AMSource::initOutput()
{
    QAudioFormat m_format;

    m_format.setSampleRate(44100);
    m_format.setChannelCount(2);
    m_format.setSampleSize(16);
    m_format.setSampleType(QAudioFormat::SignedInt);
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setCodec("audio/pcm");

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(m_format))
    {
           qDebug() << "OUTPUT Format not supported";
           return;
    }

    qDebug() << info.supportedSampleRates();

    m_audioOut = new QAudioOutput( m_format);

    m_output = m_audioOut->start();
}

