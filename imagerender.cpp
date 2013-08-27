#include "imagerender.h"
#include <QImage>
#include <QPixmap>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include "IManager.h"


ImageRender::ImageRender(QString name, QString path, QObject *parent) :
    m_name(name), QObject(parent)
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
    m_timer.start(500); // что-бы много негонять. обновление в полсекунды достаточно
}

void ImageRender::setFile(QString path)
{
    QImage img;
    m_fname = path;
    img = QImage(path);

    // запоминаем размер первой картинки и все остальные масштабируем под неё
    if(m_size.isEmpty()){
        m_size = img.size();
    }else{
        img = img.scaled(m_size);
    }

    m_image = img.convertToFormat(QImage::Format_ARGB32);
}

void ImageRender::switchImage(bool next)
{
    /* - определяем папку текущей картинки
     * - считываем названия всех image-файлов (png,jpg)
     * - определяем позицию текущего файла
     * - вычисляем позицию следующего\предыдущего файла или делаем заколцовывание
     */
    int ind;
    QFileInfo finfo(m_fname);
    QStringList name_filters;
    name_filters << "*.png" << "*.jpg" << "*.jpeg";

    QFileInfoList fns = finfo.absoluteDir().entryInfoList(name_filters,QDir::Files, next ? QDir::Name : (QDir::Name|QDir::Reversed) );

    ind = fns.indexOf(finfo) + 1;
    if(ind >= fns.length()){
        ind = 0;
    }

    qDebug() << "switch image:" << fns.at(ind).absoluteFilePath();
    setFile(fns.at(ind).absoluteFilePath());
}

void ImageRender::updateFrame()
{
    global_manager->sendExternalFrame(m_name.toLocal8Bit().data(), (char*)m_image.bits(), m_image.byteCount(), m_image.width(), m_image.height());
}

