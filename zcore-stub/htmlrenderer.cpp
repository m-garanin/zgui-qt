#include "htmlrenderer.h"
#include <QWebPage>
#include <QWebFrame>
#include <QPainter>
#include <QDebug>
#include <QUrl>

HTMLRenderer::HTMLRenderer(QObject *parent) :
    QObject(parent)
{
    m_page = new QWebPage(this);
    m_page->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    m_page->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    setSize(QSize(400, 300));

}

void HTMLRenderer::setSize(const QSize &s)
{
    m_targetSize = s;
    m_page->setPreferredContentsSize(m_targetSize);
}

void HTMLRenderer::load(const QString &url)
{
    m_image = QImage();
    QUrl url_ = QUrl::fromUserInput(url);
    qDebug() << "HTMLRenderer::load : loading URL: " << url_.toString();
    m_page->mainFrame()->load(url_);
}

QImage HTMLRenderer::render()
{
    doRender();
    return m_image;
}


void HTMLRenderer::onLoadFinished(bool ok)
{
    if (!ok) {
        qCritical() << "error loading from : " << m_page->mainFrame()->url();
    } /*else {
        doRender();
    }*/
}

void HTMLRenderer::doRender()
{
    // find proper size
    QSize size = m_page->mainFrame()->contentsSize();
    /*  qDebug() << "contentsSize: " << size << "  (" << qreal(size.width()) / size.height() << ")";
    qDebug() << "targetSize: " << m_targetSize << "  (" << qreal(m_targetSize.width()) / m_targetSize.height() << ")"*/;
    size.setHeight(size.width() * m_targetSize.height() / m_targetSize.width());
    // qDebug() << "newSize: " << size <<"  (" << qreal(size.width()) / size.height() << ")\n\n";

    m_image = QImage(size, QImage::Format_ARGB32/*QImage::Format_ARGB32_Premultiplied*/);

    m_image.fill(Qt::transparent);

    QPainter p(&m_image);
    m_page->setViewportSize(m_page->mainFrame()->contentsSize());
    m_page->mainFrame()->render(&p);
    p.end();
    m_image = m_image.scaled(m_targetSize, Qt::KeepAspectRatioByExpanding);

}
