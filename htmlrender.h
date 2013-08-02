#ifndef HTMLRENDER_H
#define HTMLRENDER_H

#include <QObject>
#include <QTimer>
#include <QWebPage>
#include <QWebFrame>

class HtmlRender : public QObject
{
    Q_OBJECT
public:
    explicit HtmlRender(QString name, QString url, QObject *parent = 0);
    void setSize(const QSize &s);
signals:
    
public slots:
    void updateFrame();

private:
    QString m_name;
    QTimer m_timer;
    QWebPage *m_page;
    QSize m_targetSize;
};

#endif // HTMLRENDER_H
