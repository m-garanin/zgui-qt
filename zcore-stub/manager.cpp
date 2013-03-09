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
    scene_count = 0;
    layer_count = 0;
}

Manager::~Manager()
{    
}

void Manager::startPipeline(int width, int height)
{
    this->width = width;
    this->height = height;

}

int Manager::addLayer(int scene_key, char *source_key, int zorder)
{
    layer_count += 1;
    int layer_id = scene_key + layer_count;
    qDebug() << "ADD LAYER " << layer_id << source_key << zorder;
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
    Q_UNUSED(compkey);

    QImage img(width, height, QImage::Format_RGB888);
    img.fill(Qt::red);
    QPainter painter(&img);
    painter.drawText(width/2, height/2, QTime::currentTime().toString());
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
