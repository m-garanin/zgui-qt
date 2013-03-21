#ifndef HTMLRENDERER_H
#define HTMLRENDERER_H

#include <QObject>
#include <QImage>
#include <QSize>
#include "irenderer.h"


class QWebPage;

class HTMLRenderer : public QObject, public IRenderer
{
    Q_OBJECT
public:
    explicit HTMLRenderer(QObject *parent = 0);
    void load(const QString &url);
    void setSize(const QSize &s);
    virtual QImage render();

private slots:
    void onLoadFinished(bool ok);

private:
    void doRender();

private:
    QWebPage *m_page;
    QSize m_targetSize;
    QImage m_image;
};

#endif // HTMLRENDERER_H
