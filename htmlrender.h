#ifndef HTMLRENDER_H
#define HTMLRENDER_H

#include <QObject>
#include <QTimer>
#include <QWebPage>
#include <QWebFrame>
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
private:
    QString m_name;
    QTimer m_timer;
    QWebPage *m_page;
    QSize m_targetSize;

    HTMLSettings* m_sett;
};

#endif // HTMLRENDER_H
