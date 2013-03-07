#include "manager.h"
#include <QImage>

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
    Q_UNUSED(compkey);

    qDebug() << "getLastImage";
    QImage img;
    img.load(QString("c:/effects/1.jpg"));
    img = img.convertToFormat(QImage::Format_RGB888);
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
