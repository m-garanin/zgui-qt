#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QTime>

#include "bkgsource.h"


BkgSource::BkgSource(QObject *parent) :
    QThread(parent)
{
}


void BkgSource::init(int w, int h){
    setSize(w,h);
    this->start();
}

void BkgSource::setBackground(QString fname)
{
    if(fname == ""){
        fname = ":WS_BKG";
    }
    m_img = QImage(fname).convertToFormat(QImage::Format_ARGB32_Premultiplied).scaled(m_width, m_height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    //
}


void BkgSource::onTimer()
{

    int width = m_width;
    int height = m_height;

    QPixmap pxm(width, height);
    const int circleRadius = width / 8;
    //img = QImage(width, height, QImage::Format_ARGB32_Premultiplied);
    //img.fill(Qt::red);
    pxm.fill(Qt::black);

    QPainter painter(&pxm);
    QFont f = painter.font();
    f.setPixelSize(width / 12);
    painter.setFont(f);
    QString text = QString("%1 - %2").arg("BKG").arg(QTime::currentTime().toString());
    QFontMetrics fm = painter.fontMetrics();
    QPen p = painter.pen();
    p.setColor(Qt::white);
    p.setWidth(2);
    painter.setPen(p);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing, true);
    painter.drawText(pxm.rect(), Qt::AlignHCenter | Qt::AlignVCenter, text);
    painter.drawEllipse(pxm.rect().topLeft() + QPoint(circleRadius, circleRadius), circleRadius, circleRadius);
    painter.drawEllipse(pxm.rect().bottomRight() - QPoint(circleRadius, circleRadius), circleRadius, circleRadius);

    //QImage img = pxm.toImage().copy();
    //emit yieldFrame(img);
    emit yieldFrame(m_img);
}
