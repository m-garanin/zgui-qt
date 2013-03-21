#include "manager.h"
#include "htmlrenderer.h"
#include <QImage>
#include <QPainter>
#include <QTime>
#include <QString>
#include <QUrl>

IManager* global_manager;

void init_core(){

    global_manager = new Manager();
}

Manager::Manager()
{
    scene_count = 0;
    layer_count = 0;
}

Manager::~Manager()
{    
    foreach (IRenderer *r, m_renderers.values())
        delete r;
    m_renderers.clear();
}

void Manager::startPipeline(int width, int height)
{
    this->width = width;
    this->height = height;
    addScene();

}

int Manager::addLayer(int scene_key, char *source_key, int zorder)
{
    layer_count += 1;
    int layer_id = scene_key + layer_count;
    qDebug() << "ADD LAYER " << layer_id << source_key << zorder;
    QString sourceKey(source_key);
    if (sourceKey.startsWith("HTML://", Qt::CaseInsensitive)) {
        if (m_renderers.contains(layer_id)) {
            qCritical() << "\tERROR : layer_id (" << layer_id << ") already exists";
        } else {

            QString url = sourceKey.section("HTML://", 1, 1);
            HTMLRenderer * r = new HTMLRenderer();
            r->setSize(QSize(width, height));
            r->load(url);
            m_renderers[layer_id] = r;
        }
    }

    return layer_id;
}

void Manager::hideLayer(int layer_id)
{
    qDebug() << "HIDE LAYER " << layer_id;
}

void Manager::showLayer(int layer_id)
{
    qDebug() << "SHOW LAYER " << layer_id;
}

void Manager::resizeLayer(int layer_id, char *pos)
{
    qDebug() << "RESIZE LAYER " << layer_id << pos;
}

void Manager::applyEffects(int layer_id, char *efnames)
{
    qDebug() << "APLLY EFFECTS TO LAYER " << layer_id << efnames;
}

void Manager::removeEffects(int layer_id)
{
    qDebug() << "REMOVE EFFECTS FROM LAYER " << layer_id;
}

void Manager::showLayerMax(int layer_id)
{
    qDebug() << "SHOW LAYER MAX " << layer_id;
}


void Manager::getLastImage(int compkey, char **ppbuf, int *pw, int *ph)
{

    QImage img;
    if (m_renderers.contains(compkey)) {
        img = m_renderers[compkey]->render();
    } else {
        //TODO: wrap it as IRenderer ?
        const int circleRadius = width / 8;
        int newHeigth = compkey % 2 == 0 ? (int)width / 4.0 * 3.0 : (int)width / 16.0 * 9.0;

        img = QImage(width, newHeigth, QImage::Format_RGB888);
        // сцены наполняем красным, слои - цианом.
        if(compkey % 100 == 0){
            img.fill(Qt::red);
        }else{
            img.fill(Qt::cyan);
        }
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
    }
    *pw = img.width();
    *ph = img.height();
    *ppbuf = (char*)malloc(img.byteCount());
    memcpy(*ppbuf, img.bits(), img.byteCount() );
}


int Manager::addScene()
{
    scene_count += 100;
    return scene_count;
}


bool Manager::addAudioSource(char *source_key)
{
    QString src = QString(source_key);
    if(audios.contains(src)){
        qDebug() << "ADD AUDIO ALREADY EXITS" << src;
        return false;
    }
    audios.append(src);
    qDebug() << "ADD AUDIO " << src;
    return true;
}

void Manager::toggleMute(char *srcname)
{
    qDebug() << "TOGGLE MUTE" << srcname;
}

void Manager::setVolume(char *srcname, double vol)
{
    qDebug() << "SET VOLUME" << srcname << vol;
}
