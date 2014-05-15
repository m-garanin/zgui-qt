#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QTime>

#include "testsource.h"


TestSource::TestSource(QObject *parent) :
    QThread(parent)
{
}

void TestSource::init(int w, int h)
{
    m_width = w;
    m_height = h;
    this->start();
}


void TestSource::onTimer()
{
    QImage img;

    int width = m_width;
    int height = m_height;

    const int circleRadius = width / 8;
    img = QImage(width, height, QImage::Format_ARGB32_Premultiplied);
    img.fill(Qt::black);

    QPainter painter(&img);
    QFont f = painter.font();
    f.setPixelSize(width / 20);
    painter.setFont(f);
    QString text = QString("%1 - %2").arg((long)(this)).arg(QTime::currentTime().toString());
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
