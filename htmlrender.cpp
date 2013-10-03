#include "htmlrender.h"
#include "IManager.h"
#include <QDebug>
#include <QFile>
#include <QWebElement>
#include "settingsmanager.h"
#include "utils.h"

HtmlRender::HtmlRender(QString name, QString fname, QObject *parent) :
    m_name(name),
    QObject(parent)
{
    m_page = new QWebPage(this);
    m_page->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    m_page->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);

    QFile file(fname);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    connect(m_page->mainFrame(), SIGNAL(loadFinished(bool)), this, SLOT(onLoad(bool)));

    // получаем размеры рабочей зоны    
    QStringList sz = getWorksize().split("x");
    // формируем URL
    QString qstring = "?without_settings=1&width=" + sz[0] + "&height=" + sz[1];
    m_page->mainFrame()->setHtml(file.readAll(), QUrl::fromLocalFile(fname + qstring));


    m_sett = new HTMLSettings();
    connect(m_sett, SIGNAL(change_params(QString)), this, SLOT(onChangeParams(QString)));

    m_sett->openURL(QUrl::fromLocalFile(fname));
}

void HtmlRender::setSize(const QSize &s)
{
    m_targetSize = s;
    m_page->setPreferredContentsSize(m_targetSize);

}



void HtmlRender::onLoad(bool flag)
{
    QSize size = m_page->mainFrame()->contentsSize();
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

    //connect(m_page, SIGNAL(repaintRequested(QRect)), this, SLOT(onRepaintRequested(QRect)));
    //
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
    m_timer.start(40);
}

void HtmlRender::onChangeParams(QString params)
{
    m_params = params;
    callScript();
}

void HtmlRender::onShowSignal()
{
    callScript();
}

void HtmlRender::onDeleteLayer()
{
    m_timer.stop();
    m_page->deleteLater();
}


void HtmlRender::callScript()
{
    QString call;
    if(m_params.isEmpty()){
        call = "Z_RUN()";
    }else{
        call = "Z_APPLY(" + m_params + ")";
    }
    m_page->mainFrame()->documentElement().evaluateJavaScript(call);
}

void HtmlRender::onHTMLPluginSettings()
{
    m_sett->show();
}



void HtmlRender::onRepaintRequested(QRect rec)
{
    //m_page->mainFrame()->render(&m_painter, rec);

}

void HtmlRender::updateFrame()
{
    m_img.fill(Qt::transparent);
    m_page->mainFrame()->render(&m_painter);
    QImage& img = m_img;
    //qDebug() << img.size() << img.isNull();
    global_manager->sendExternalFrame(m_name.toLocal8Bit().data(), (char*)img.bits(), img.byteCount(), img.width(), img.height());


 }

