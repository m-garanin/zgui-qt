#ifndef HTMLSOURCE_H
#define HTMLSOURCE_H

#include <QThread>
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QWebPage>
#include <QWebFrame>

#include "htmlsettings.h"

class HtmlSource : public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE{
        forever{
            if ( QThread::currentThread()->isInterruptionRequested() )
                 return;
            onTimer();
            msleep(40);
        }

    }
public:
    explicit HtmlSource(QObject *parent = 0);

    void init(QString fname, int w, int h);
signals:
    void  yieldFrame(const QImage&);

public slots:
    void onLoad(bool flag);
    void onRepaintRequested(QRect rec);
    void onChangeParams(QString params);
    void onHTMLPluginSettings();
    void onShowSignal();

private:
    QPixmap m_pxm;
    QWebPage *m_page;
    QPainter m_painter;
    QString m_params;
    HTMLSettings* m_sett;
    void callScript();

    void onTimer();
};

#endif // HTMLSOURCE_H
