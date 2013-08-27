#include "imagerender.h"
#include <QImage>
#include <QPixmap>
#include <QFileInfo>
#include <QDir>
#include <QPainter>
#include <QDebug>
#include "IManager.h"


ImageRender::ImageRender(QString name, QString path, QObject *parent) :
    m_name(name), QObject(parent)
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
    m_timer.start(500); // что-бы много негонять. такое обновление достаточно
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
        QImage bg(m_size,QImage::Format_ARGB32);
        QPainter p;
        p.begin(&bg);
        img = img.scaled(m_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        // позиционирование по середине
        int x,y;
        if(img.width() == m_size.width()){
            x = 0;
            y = (m_size.height() - img.height())/2;
        }else{
            x = (m_size.width() - img.width())/2;
            y = 0;
        }

        p.drawImage(x,y, img);
        p.end();
        img = bg;
    }

    m_image = img.convertToFormat(QImage::Format_ARGB32);

    //
    updateFrame();
    emit newFile(m_fname);
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

    setFile(fns.at(ind).absoluteFilePath());
}

void ImageRender::updateFrame()
{
    global_manager->sendExternalFrame(m_name.toLocal8Bit().data(), (char*)m_image.bits(), m_image.byteCount(), m_image.width(), m_image.height());
}

