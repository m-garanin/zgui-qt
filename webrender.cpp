#include <QDebug>
#include "webrender.h"
#include "utils.h"

WEBRender::WEBRender(QString name, QString uri, QObject *parent) :
    m_name(name),
    m_pOut(NULL),
    QObject(parent)
{
    m_page = new QWebPage(this);
    m_page->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    m_page->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);

    connect(m_page->mainFrame(), SIGNAL(loadFinished(bool)), this, SLOT(onLoad(bool)));

    // получаем размеры рабочей зоны
    QStringList sz = getWorksize().split("x");
    // формируем URL
    //QString qstring = "?without_settings=1&width=" + sz[0] + "&height=" + sz[1];
    //m_page->mainFrame()->setHtml(file.readAll(), QUrl::fromLocalFile(fname + qstring));

    m_page->mainFrame()->load(QUrl(uri));
}

void WEBRender::updateFrame()
{
    m_img.fill(Qt::transparent);
    m_page->mainFrame()->render(&m_painter);
    QImage& img = m_img;
    //qDebug() << img.size() << img.isNull();

    if(m_pOut==NULL){
        global_manager->queryIExternalSource(m_name.toLocal8Bit().data(), &m_pOut);
    }

    m_pOut->sendFrame((char*)img.bits(), img.byteCount(), img.width(), img.height());

}

void WEBRender::onRepaintRequested(QRect rec)
{
    //qDebug() << "ON REPAINT" << rec;
    //m_img.fill(Qt::transparent);
    m_page->mainFrame()->render(&m_painter, rec);
    QImage& img = m_img;
    //qDebug() << img.size() << img.isNull();

    if(m_pOut==NULL){
        global_manager->queryIExternalSource(m_name.toLocal8Bit().data(), &m_pOut);
    }

    m_pOut->sendFrame((char*)img.bits(), img.byteCount(), img.width(), img.height());

}


void WEBRender::onLoad(bool flag)
{
    QSize size = m_page->mainFrame()->contentsSize();

    qDebug() << "ON LOAD: " << flag << size;
    m_page->setViewportSize(size);
    m_img = QImage(size, QImage::Format_ARGB32);
    m_img.fill(Qt::transparent);

    QPalette pal = m_page->palette();
    pal.setBrush(QPalette::Base, Qt::transparent);
    m_page->setPalette(pal);

    m_painter.begin(&m_img);

    m_painter.setRenderHint(QPainter::Antialiasing, true);
    m_painter.setRenderHint(QPainter::TextAntialiasing, true);
    m_painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    connect(m_page, SIGNAL(repaintRequested(QRect)), this, SLOT(onRepaintRequested(QRect)));
    //
    //connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
    //m_timer.start(40);
}

void WEBRender::onDeleteLayer()
{
    m_timer.stop();
    m_page->deleteLater();
}
