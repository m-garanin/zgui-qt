#include <QDebug>

#include <QDir>
#include <QPainter>
#include <QFileDialog>

#include "settingsmanager.h"


#include "imagesource.h"



ImageSource::ImageSource(QObject *parent) :
    QObject(parent)
{
}

void ImageSource::setFile(QString path)
{
    m_fname = path;
    m_image = QImage(path).convertToFormat(QImage::Format_ARGB32_Premultiplied);

    emit yieldFrame(m_image);
    emit newFile(m_fname);
}

void ImageSource::switchImage(bool next)
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

void ImageSource::selectImage()
{
    SettingsManager settings("MainWindow");
    QString file = QFileDialog::getOpenFileName(NULL, tr("Select Image"), settings.getStringValue("default_dir"), "Image Files (*.png *.jpg *.bmp)");
    if (!file.isEmpty())
    {
        QDir curDir(file);
        settings.setValue("default_dir", curDir.absolutePath());
        //this->addImageLayer(file);
        setFile(file);
    }
}
