#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
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
    QObject* addSource(QString type, QString sourcename, QString ainfo);

signals:

public slots:

private:
    BkgSource* m_bkg;
};

#endif // MANAGER_H
