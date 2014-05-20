#include <QMessageBox>

#include "manager.h"
#include "testsource.h"
#include "imagesource.h"
#include "screensource.h"
#include "htmlsource.h"
#include "xmsource.h"

void app_logger(char* buf){
    global_manager->logger(buf);
}

Manager::Manager(QObject *parent) :
    QObject(parent), m_xmgr(NULL)
{
}

void Manager::start(int width, int height)
{
    m_size = QSize(width, height);
    m_bkg = new BkgSource();
    m_bkg->init(width, height);
}

void Manager::stop()
{

}

void Manager::logger(char *buf)
{
    QString msg = QString(buf);

    emit log(msg);
}

Scene *Manager::addScene()
{
    Scene* s = new Scene(m_size);
    connect(m_bkg, SIGNAL(yieldFrame(const QImage&)), s, SLOT(onBkgFrame(const QImage&)));
    return s;
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

void Manager::initXManager()
{
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

    m_xmgr->init( (app_buffer_callback)xm_buffer_callback, (app_logger_callback)app_logger);

}

void Manager::addCam(QString source_name, QSize ainfo)
{
    if(source_name == "VS-A" or source_name == "VS-B"){
        TestSource* src = new TestSource();
        src->init(m_size.width(), m_size.height());
        m_sources[source_name] = src;
        return;
    }

    if(m_xmgr == NULL){
        initXManager();
    }

    XMSource* src = new XMSource();

    m_xmgr->createCamSource(source_name.toLocal8Bit().data(), m_size.width(), m_size.height(), src); // TODO: ainfo

    m_sources[source_name] = src;
}
