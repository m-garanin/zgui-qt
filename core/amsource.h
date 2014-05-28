#ifndef AMSOURCE_H
#define AMSOURCE_H

#include <QAudioOutput>
#include <QAudioDeviceInfo>

#include "xmsource.h"

class AMSource : public XMSource
{
    Q_OBJECT
public:
    explicit AMSource(QObject *parent = 0);

    void buffer_callback(int type, char* buffer, int w, int h, int size);

signals:

public slots:

private:
    QIODevice *m_output;
    QAudioOutput* m_audioOut;

    void initOutput();
};

#endif // AMSOURCE_H
