#include <QMessageBox>

#include "manager.h"
#include "testsource.h"
#include "imagesource.h"
#include "screensource.h"
#include "htmlsource.h"

static Manager* priv_manager;

void buffer_callback(int type, char *buffer, uint64 ts, uint64 drt, int w, int h, int size, void *userdata)
{

    if(userdata == NULL){
        priv_manager->master_buffer_callback(type, buffer, ts, drt, w, h, size);
        return;
    }

    XMSource* psrc = (XMSource*)userdata;
    psrc->buffer_callback(type, buffer, w, h, size); // для XMSource ts не нужен
}


void app_logger(char* buf){
    global_manager->logger(buf);
}

Manager::Manager(QObject *parent) :
    QObject(parent), m_xmgr(NULL), m_rec(false), m_air(false), m_main_scene(NULL)
{
    priv_manager = this;
}

void Manager::start(int width, int height)
{

    initAudioOutput();

    initXManager();

    m_size = QSize(width, height);
    m_main_scene = new Scene(m_size);

    m_main_scene->setSize(width, height);

}

void Manager::stop()
{

}

void Manager::logger(char *buf)
{
    QString msg = QString(buf);

    emit log(msg);
}


QObject* Manager::addSource(QString type, QString source_name, QVariant ainfo)
{
    QObject* res;
    if(m_sources.contains(source_name))
        goto end;

    if( type == "CAM"){
        addCam(source_name, ainfo.toSize());
        goto end;
    }

    if( type == "VIDEOFILE"){
        addVideoFile(source_name);
        goto end;
    }

    if( type == "NETSOURCE"){
        addNetSource(source_name);
        goto end;
    }

    if( type == "IMAGE"){
        m_sources[source_name] = new ImageSource();
        goto end;
    }

    if( type == "SCREEN"){
        ScreenSource* src = new ScreenSource();
        src->init(ainfo.toRect());
        m_sources[source_name] = src;
        goto end;
    }

    if( type == "HTML"){
        HtmlSource* src = new HtmlSource();
        src->init(source_name, m_size.width(), m_size.height());
        m_sources[source_name] = src;
        goto end;
    }
end:
    res = m_sources[source_name];

    return res;
}

void Manager::master_buffer_callback(int type, char *buffer, uint64 ts, uint64 drt, int w, int h, int size)
{
    if(type == AUDIO_TYPE){
        mixAudio(buffer, ts, drt, size);
    }else{
        mixVideo(ts, drt);
    }

}


void Manager::initXManager()
{
    if(m_xmgr != NULL)
        return;

    // получает IXManager из библиотеки

    // загрузка функции из zcore (.dll, dylib, so)
    m_xmgr_lib.setFileName("xmanager"); // qt само поставляет dll | dylib | so

    bool r = m_xmgr_lib.load();

    if (!r) {
        qDebug() << "ERROR LOAD XMANAGER LIBRARY:" << m_xmgr_lib.errorString();

        // возможно нужно бросать сигнал
        //TODO: QMessageBox::critical(this, "ERROR LOAD XMANAGER LIBRARY", m_zcoreLib.errorString() ) ;
    }

    XMANAGER_GET_IFACE iface_query =(XMANAGER_GET_IFACE)m_xmgr_lib.resolve("getIXManager");
    Q_ASSERT(iface_query);
    iface_query(&m_xmgr);

    Q_ASSERT(m_xmgr != NULL);

    m_xmgr->init( (app_buffer_callback)buffer_callback, (app_logger_callback)app_logger);

}


void Manager::addCam(QString source_name, QSize ainfo)
{
    if(source_name == "VS-A" or source_name == "VS-B"){
        TestSource* src = new TestSource();
        src->init(m_size.width(), m_size.height());
        m_sources[source_name] = src;
        return;
    }

    XMSource* src = new XMSource();

    m_xmgr->createCamSource(source_name.toLocal8Bit().data(), m_size.width(), m_size.height(), src); // TODO: ainfo

    m_sources[source_name] = src;
}

void Manager::addVideoFile(QString fname)
{
    qDebug() << "ADD VIDEO FILE" << fname;


    XMSource* src = new XMSource();
    IPlaybackSource* pbk;

    m_xmgr->createVideoFileSource(fname.toLocal8Bit().data(), &pbk, src);
    m_sources[fname] = src;
    // добавляем в аудио
    m_audios.append(src);
}

void Manager::addNetSource(QString uri)
{
    qDebug() << "ADD NET SOURCE" << uri;

    XMSource* src = new XMSource();
    IPlaybackSource* pbk;

    m_xmgr->createNetSource(uri.toLocal8Bit().data(), src);
    m_sources[uri] = src;

    // добавляем в аудио
    m_audios.append(src);
}

void Manager::initAudioOutput()
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

    m_aoutput = m_audioOut->start();
}





void Manager::startRec(char* fname, int width, int height, int vbr, int ar){
    m_xmgr->startRec(fname, width, height, vbr, ar);
    m_rec = true;
}

void Manager::stopRec(){
    m_rec = false;
    m_xmgr->stopRec();
}

void Manager::startAir(int ch_id, QString pwd, QString param_fname, QString server, QString log_fname, int width, int height, int bitrate, char tarif, char quality, int acc, int test)
{
    m_air_mgr.startAir(ch_id, pwd, param_fname, server, log_fname, width, height, bitrate, tarif, quality, acc, test);
    m_air = true;
}

void Manager::stopAir()
{
    m_air = false;
    m_air_mgr.stopAir();
}



void Manager::mixVideo(uint64 ts, uint64 drt)
{
    QImage img = m_main_scene->makeMix();

    //qDebug() << "MIX VIDEO";
    if(m_rec){
        m_xmgr->sendRecBuffer(VIDEO_TYPE, (char*)img.bits(), img.byteCount(), ts, drt);
    }

    if(m_air){
        m_air_mgr.sendVideo(img, ts);
    }
}


void Manager::mixAudio(char *buffer, uint64 ts, uint64 drt, int size)
{
    int hsize;
    static char tbuf[100000];
    qint16* sbuf = (qint16*)buffer;
    qint16* tsb = (qint16*)tbuf;

    // бежим по slave-потокам
    foreach (XMSource* src, m_audios) {
        hsize = src->m_abuffer.length();
        if(hsize == 0)
            continue;

        // берём данные из slave-потока, но не более чем
        // размер текущего мастер-буфера
        if(hsize > size) {
            hsize = size;
        }

        src->m_amutex.lock();
        memcpy(tbuf, src->m_abuffer.data(), hsize);
        src->m_abuffer.remove(0, hsize);
        src->m_amutex.unlock();

        for(int i=0; i<hsize/2;i++){
            sbuf[i] = AUDIO_CLAMP_SW( sbuf[i] + tsb[i] );
        }
    }

    m_aoutput->write(buffer, size);

    if(m_rec){
        m_xmgr->sendRecBuffer(AUDIO_TYPE, buffer, size, ts, drt);
    }


    if(m_air){
        m_air_mgr.sendAudio((uint8_t*)buffer, size, ts);
    }

}














