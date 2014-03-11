#ifndef WEBRENDER_H
#define WEBRENDER_H

#include <QObject>
#include <QTimer>
#include <QPainter>
#include <QImage>
#include <QWebPage>
#include <QWebFrame>
#include "IManager.h"

class WEBRender : public QObject
{
    Q_OBJECT
public:
    explicit WEBRender(QString name, QString uri, QObject *parent = 0);
signals:
    
public slots:
    void updateFrame();
    void onRepaintRequested(QRect rec);
    void onLoad(bool flag);
    void onDeleteLayer();
private:
    IExternalSource* m_pOut;
    QString m_name;
    QTimer m_timer;
    QWebPage *m_page;
    QImage m_img;
    QPainter m_painter;

};

#endif // WEBRENDER_H
