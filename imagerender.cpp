#include "imagerender.h"
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include "IManager.h"

ImageRender::ImageRender(QString name, QString path, QObject *parent) :
    m_name(name), QObject(parent)
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
    m_timer.start(40);
}

void ImageRender::setFile(QString path)
{
    m_image = QImage(path).convertToFormat(QImage::Format_ARGB32);

    //global_manager->sendExternalFrame(m_name.toLocal8Bit().data(), (char*)m_image.bits(), m_image.byteCount(), m_image.width(), m_image.height());
    // TODO: второй вызов - это костыль, нужно убрать
    //global_manager->sendExternalFrame(m_name.toLocal8Bit().data(), (char*)m_image.bits(), m_image.byteCount(), m_image.width(), m_image.height());

}

void ImageRender::updateFrame()
{
    global_manager->sendExternalFrame(m_name.toLocal8Bit().data(), (char*)m_image.bits(), m_image.byteCount(), m_image.width(), m_image.height());
}

