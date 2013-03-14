#include "previewwidget.h"
#include <QPainter>
#include <QFont>
#include <QTimer>

#include <QDebug>
#include <QPixmap>

#include "IManager.h"

void myImageCleanupHandler(void *info){
    free(info);
}

PreviewWidget::PreviewWidget(qint32 compkey, QWidget *parent) :
    m_compkey(compkey), QWidget(parent),m_currentImage(NULL), m_imageFitMode(ImageFit)
{
    start();
    setAcceptDrops(true);
}

void PreviewWidget::start()
{
    // заводим таймер
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePreview()));
    timer->start(40); // соответствует 25 FPS
}

void PreviewWidget::setImageFitMode(PreviewWidget::ImageFitMode mode)
{
    m_imageFitMode = mode;
    update();
}

void PreviewWidget::updatePreview()
{
    if(m_compkey == 0)
        return;

    char* buf = NULL;
    int w,h;
    global_manager->getLastImage(m_compkey, &buf, &w, &h);
    QImage* pimg = new QImage((uchar*)buf, w, h, QImage::Format_RGB888, &myImageCleanupHandler, buf);
    drawImage(pimg);

}

void PreviewWidget::drawImage(QImage *img)
{
    if(m_currentImage != NULL && m_currentImage != img)
        delete m_currentImage;
    m_currentImage = img;

    this->update();
}

QImage PreviewWidget::image() const
{
    // TODO:
    // temp solution
    // in future copy m_currentImage
    QImage newImage(rect().size(), QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    return newImage;
}

void PreviewWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.save(); // BBB: why ?
    painter.fillRect(0,0, width(), height(), Qt::black); // background. TODO: может есть более правильный способ?

    if(m_currentImage == NULL){
        painter.fillRect(0,0, width(), height(), Qt::black);
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 20));
        painter.drawText(rect(), Qt::AlignCenter, "wait...");

    } else {
        QImage img;
        QPoint origin;
        switch (m_imageFitMode) {
        case ImageFit:
            img = m_currentImage->scaled(this->size(), Qt::KeepAspectRatio);
            origin.setX((this->width() - img.width()) / 2);
            origin.setY((this->height() - img.height()) / 2);
            break;
        case ImageStretch:
            img  = m_currentImage->scaled(this->size());
            break;
        };
        painter.drawImage(origin, img);
    }
    painter.restore();
}


