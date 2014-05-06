#include "manager.h"
#include "testsource.h"
#include "imagesource.h"

Manager::Manager(QObject *parent) :
    QObject(parent)
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

Scene *Manager::addScene()
{
    Scene* s = new Scene();
    connect(m_bkg, SIGNAL(yieldFrame(const QImage&)), s, SLOT(onBkgFrame(const QImage&)));
    return s;
}

QObject* Manager::addSource(QString type, QString source_name, QString ainfo)
{
    QObject* res;
    if(m_sources.contains(source_name))
        goto end;

    if( type == "CAM"){
        addCam(source_name, ainfo);
        goto end;
    }

    if( type == "IMAGE"){
        m_sources[source_name] = new ImageSource();
        goto end;
    }

end:
    res = m_sources[source_name];

    return res;
}

void Manager::addCam(QString source_name, QString ainfo)
{
    if(source_name == "VS-A" or source_name == "VS-B"){
        TestSource* src = new TestSource();
        src->init(m_size.width(), m_size.height());
        m_sources[source_name] = src;
        return;
    }

}
