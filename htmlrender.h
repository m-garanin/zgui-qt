#ifndef HTMLRENDER_H
#define HTMLRENDER_H

#include <QObject>
#include <QTimer>
#include <QPainter>
#include <QImage>
#include <QWebPage>
#include <QWebFrame>
#include <QQueue>
#include "htmlsettings.h"

class HtmlRender : public QObject
{
    Q_OBJECT
public:
    explicit HtmlRender(QString name, QString fname, QObject *parent = 0);
    void setSize(const QSize &s);
signals:
    
public slots:    
    void updateFrame();
    void onLoad(bool flag);
    void onRepaintRequested(QRect rec);
    void onChangeParams(QString params);
    void onHTMLPluginSettings();
    void onShowSignal();
    void onDeleteLayer();
private:
    QString m_name;
    QString m_params;
    QTimer m_timer;
    QWebPage *m_page;
    QSize m_targetSize;
    QImage m_img;
    QPainter m_painter;
    QQueue<QImage> m_frames;
    HTMLSettings* m_sett;

    void callScript();
};

#endif // HTMLRENDER_H
