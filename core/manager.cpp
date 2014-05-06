#include "manager.h"

Manager::Manager(QObject *parent) :
    QObject(parent)
{
}

void Manager::start(int width, int height)
{
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

QObject *Manager::addSource(QString type, QString sourcename, QString ainfo)
{
    return m_bkg;
}
