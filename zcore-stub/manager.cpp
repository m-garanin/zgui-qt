#include "manager.h"
#include <QImage>
#include <QPainter>
#include <QTime>

IManager* global_manager;

void init_core(){

    global_manager = new Manager();
}

Manager::Manager()
{

}

Manager::~Manager()
{    
}

void Manager::startPipeline(int width, int height)
{    
    qDebug() << "START PIPELINE ZZZ" << width << "x" << height;
    this->width = width;
    this->height = height;

}

int Manager::addLayer(int scene_key, char *source_key, int zorder)
{
    return 110;
}

void Manager::getLastImage(int compkey, char **ppbuf, int *pw, int *ph)
{
    //Q_UNUSED(compkey);

    qDebug() << "getLastImage";
    const int circleRadius = width / 8;
    int newHeigth = compkey % 2 == 0 ? (int)width / 4.0 * 3.0 : (int)width / 16.0 * 9.0;

    QImage img(width, newHeigth, QImage::Format_RGB888);
    img.fill(Qt::red);
    QPainter painter(&img);
    QFont f = painter.font();
    f.setPixelSize(width / 12);
    painter.setFont(f);
    QString text = QString("%1 - %2").arg(compkey).arg(QTime::currentTime().toString());
    QFontMetrics fm = painter.fontMetrics();
    QSize textSize = fm.size(Qt::TextSingleLine, text);
    QPen p = painter.pen();
    p.setWidth(2);
    painter.setPen(p);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing, true);
    painter.drawText(img.rect(), Qt::AlignHCenter | Qt::AlignVCenter, text);
    painter.drawEllipse(img.rect().topLeft() + QPoint(circleRadius, circleRadius), circleRadius, circleRadius);
    painter.drawEllipse(img.rect().bottomRight() - QPoint(circleRadius, circleRadius), circleRadius, circleRadius);
    *pw = img.width();
    *ph = img.height();
    *ppbuf = (char*)malloc(img.byteCount());    
    memcpy(*ppbuf, img.bits(), img.byteCount() );
}

int Manager::addScene()
{
    return 100;
}

void Manager::addSource(char *key)
{
     qDebug()<< "addSource:" << key;
}

int Manager::test()
{
    return 12;
}
