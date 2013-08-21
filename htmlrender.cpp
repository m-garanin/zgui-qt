#include "htmlrender.h"
#include "IManager.h"
#include <QImage>
#include <QPainter>
#include <QDebug>
#include <QFile>


HtmlRender::HtmlRender(QString name, QString fname, QObject *parent) :
    m_name(name),
    QObject(parent)
{
    m_page = new QWebPage(this);
    m_page->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    m_page->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);

    QFile file(fname);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    m_page->mainFrame()->setHtml(file.readAll());

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
    m_timer.start(33);

    m_sett = new HTMLSettings();
    m_sett->show();

    m_sett->openURL(QUrl::fromLocalFile(fname));

}

void HtmlRender::setSize(const QSize &s)
{
    m_targetSize = s;
    m_page->setPreferredContentsSize(m_targetSize);

}

void HtmlRender::updateFrame()
{    
    QSize size = m_page->mainFrame()->contentsSize();
    QImage img = QImage(size, QImage::Format_ARGB32);
    img.fill(Qt::transparent);


    QPalette pal = m_page->palette();
    pal.setBrush(QPalette::Base, Qt::transparent);
    m_page->setPalette(pal);


    QPainter p(&img);
    m_page->setViewportSize(size);

    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::TextAntialiasing, true);
    p.setRenderHint(QPainter::SmoothPixmapTransform, true);    
    m_page->mainFrame()->render(&p);
    p.end();

    //m_sett->showFrame(img);
    global_manager->sendExternalFrame(m_name.toLocal8Bit().data(), (char*)img.bits(), img.byteCount(), img.width(), img.height());

}
