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
    m_width = w;
    m_height = h;
    this->start();
}


void BkgSource::onTimer()
{
    QImage img;

    int width = m_width;
    int height = m_height;

    const int circleRadius = width / 8;
    img = QImage(width, height, QImage::Format_ARGB32_Premultiplied);
    img.fill(Qt::red);

    QPainter painter(&img);
    QFont f = painter.font();
    f.setPixelSize(width / 12);
    painter.setFont(f);
    QString text = QString("%1 - %2").arg("BKG").arg(QTime::currentTime().toString());
    QFontMetrics fm = painter.fontMetrics();
    QSize textSize = fm.size(Qt::TextSingleLine, text);
    QPen p = painter.pen();
    p.setWidth(2);
    painter.setPen(p);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing, true);
    painter.drawText(img.rect(), Qt::AlignHCenter | Qt::AlignVCenter, text);
    painter.drawEllipse(img.rect().topLeft() + QPoint(circleRadius, circleRadius), circleRadius, circleRadius);
    painter.drawEllipse(img.rect().bottomRight() - QPoint(circleRadius, circleRadius), circleRadius, circleRadius);


    emit yieldFrame(img);


}
