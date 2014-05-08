#include <QFile>
#include <QWebElement>
#include "htmlsource.h"

HtmlSource::HtmlSource(QObject *parent) :
    QThread(parent)
{
}

void HtmlSource::init(QString fname, int w, int h)
{
    m_page = new QWebPage(this);
    m_page->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    m_page->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);

    QFile file(fname);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    connect(m_page->mainFrame(), SIGNAL(loadFinished(bool)), this, SLOT(onLoad(bool)));

    // формируем URL
    QString qstring = QString("?without_settings=1&width=%1&height=%2").arg(w).arg(h);
    m_page->mainFrame()->setHtml(file.readAll(), QUrl::fromLocalFile(fname + qstring));


    m_sett = new HTMLSettings();
    connect(m_sett, SIGNAL(change_params(QString)), this, SLOT(onChangeParams(QString)));

    m_sett->openURL(QUrl::fromLocalFile(fname));

    this->start();
}

void HtmlSource::onLoad(bool flag)
{
    QSize size = m_page->mainFrame()->contentsSize();
    m_page->setViewportSize(size);

    m_pxm = QPixmap(size);
    m_pxm.fill(Qt::transparent);

    QPalette pal = m_page->palette();
    pal.setBrush(QPalette::Base, Qt::transparent);
    m_page->setPalette(pal);

    m_painter.begin(&m_pxm);

    m_painter.setRenderHint(QPainter::Antialiasing, true);
    m_painter.setRenderHint(QPainter::TextAntialiasing, true);
    m_painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    connect(m_page, SIGNAL(repaintRequested(QRect)), this, SLOT(onRepaintRequested(QRect)));
}

void HtmlSource::onRepaintRequested(QRect rec)
{
    //qDebug() << "ON REPAINT" << rec;
}

void HtmlSource::onChangeParams(QString params)
{
    m_params = params;
    callScript();
}

void HtmlSource::onHTMLPluginSettings()
{
    m_sett->show();
}

void HtmlSource::onShowSignal()
{
    callScript();
}

void HtmlSource::callScript()
{
    QString call;
    if(m_params.isEmpty()){
        call = "Z_RUN()";
    }else{
        call = "Z_APPLY(" + m_params + ")";
    }
    m_page->mainFrame()->documentElement().evaluateJavaScript(call);

}

void HtmlSource::onTimer()
{
    m_pxm.fill(Qt::transparent);
    m_page->mainFrame()->render(&m_painter);

    QImage img = m_pxm.toImage();
    emit yieldFrame(img.copy());
}
