#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QLibrary>
#include <QHash>
#include <QList>

#include <QAudioOutput>
#include <QAudioDeviceInfo>

#include "IManager.h"
#include "XInterfaces.h"
#include "xmsource.h"
#include "airmanager.h"

typedef void (__cdecl *XMANAGER_GET_IFACE)(IXManager**);

class Manager : public QObject, public IManager
{
    Q_OBJECT
public:
    explicit Manager(QObject *parent = 0);
    void start(int width, int height);
    void stop();
    void logger(char* buf);

    QObject* addSource(QString type, QString sourcename, QVariant ainfo);

    Scene* getScene() {return m_main_scene;}
    void setWorksize(int w, int h) {m_main_scene->setSize(w,h);}
    void setBackground(QString fname) { m_main_scene->setBackground(fname);}

    // rec
    void startRec(char* fname, int width, int height, int vbr, int ar);
    void stopRec();

    // air
    void startAir(int ch_id, char* pwd,
                          char* param_fname, char* server, char* log_fname,
                          int width, int height, int bitrate, char tarif, char quality, int acc,
                          int test);

    void stopAir();




    void master_buffer_callback(int type, char* buffer, uint64 ts, uint64 drt, int w, int h, int size);
signals:
    void log(QString msg);

public slots:

private:
    QLibrary m_xmgr_lib;
    IXManager* m_xmgr;

    Scene* m_main_scene;
    QSize m_size;
    QHash<QString, QObject*> m_sources;
    bool m_rec;
    bool m_air;
    AIRManager m_air_mgr;

    void initXManager();
    void initAudio();
    void addCam(QString source_name, QSize ainfo);
    void addVideoFile(QString fname);
    void addNetSource(QString uri);

    //////////////////////////////
    // AUDIO
    QList<XMSource*> m_audios;
    QIODevice *m_aoutput;
    QAudioOutput* m_audioOut;

    void initAudioOutput();
    void mixAudio(char* buffer, uint64 ts, uint64 drt, int size);

    void mixVideo(uint64 ts, uint64 drt);
};


#define AUDIO_CLAMP(x,a,b) ((x)<(a) ? (a) : ((x)>(b) ? (b) : (x)))
#define AUDIO_SW_MAX 32767
#define AUDIO_SW_MIN (-1-AUDIO_SW_MAX)
#define AUDIO_CLAMP_SW(x) AUDIO_CLAMP(x,AUDIO_SW_MIN,AUDIO_SW_MAX)




#endif // MANAGER_H
