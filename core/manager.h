#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QLibrary>
#include <QHash>
#include "IManager.h"
#include "XInterfaces.h"
#include "bkgsource.h"

typedef void (__cdecl *XMANAGER_GET_IFACE)(IXManager**);

class Manager : public QObject, public IManager
{
    Q_OBJECT
public:
    explicit Manager(QObject *parent = 0);
    void start(int width, int height);
    void stop();
    void logger(char* buf);

    Scene* addScene();
    QObject* addSource(QString type, QString sourcename, QVariant ainfo);


    void setWorksize(int w, int h) {m_bkg->setSize(w,h);}
    void setBackground(QString fname) { m_bkg->setBackground(fname);}


signals:
    void log(QString msg);

public slots:

private:
    QLibrary m_xmgr_lib;
    IXManager* m_xmgr;
    BkgSource* m_bkg;
    QSize m_size;
    QHash<QString, QObject*> m_sources;

    void initXManager();
    void initAudio();
    void addCam(QString source_name, QSize ainfo);
    void addVideoFile(QString fname);
    void addNetSource(QString uri);
};

#endif // MANAGER_H
