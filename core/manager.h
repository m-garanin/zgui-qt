#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QHash>
#include "IManager.h"
#include "bkgsource.h"

class Manager : public QObject, public IManager
{
    Q_OBJECT
public:
    explicit Manager(QObject *parent = 0);
    void start(int width, int height);
    void stop();


    Scene* addScene();
    QObject* addSource(QString type, QString sourcename, QVariant ainfo);


    void setWorksize(int w, int h) {m_bkg->setSize(w,h);}
    void setBackground(QString fname) { m_bkg->setBackground(fname);}


signals:

public slots:

private:
    BkgSource* m_bkg;
    QSize m_size;
    QHash<QString, QObject*> m_sources;


    void addCam(QString source_name, QSize ainfo);
};

#endif // MANAGER_H
