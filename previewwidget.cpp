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
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePreview()));
    start();
    setAcceptDrops(true);
}

void PreviewWidget::start()
{
    // заводим таймер
    if(!timer->isActive())
    {
        timer->start(40); // соответствует 25 FPS
    }
}

void PreviewWidget::stop()
{
    timer->stop();
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

/*
QImage PreviewWidget::image()
{
    return QPixmap::grabWidget(this).toImage();
}
*/

void PreviewWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.fillRect(0,0, width(), height(), Qt::black); // background. TODO: может есть более правильный способ?

    if(m_currentImage == NULL){
        painter.fillRect(0,0, width(), height(), Qt::black);
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 20));
        painter.drawText(rect(), Qt::AlignCenter, "wait...");

    } else {
        QPixmap spm;
        QPixmap pxm;
        QPoint origin;
        pxm = QPixmap::fromImage(QImage(*m_currentImage));

        switch (m_imageFitMode) {
        case ImageFit:
            spm = pxm.scaled(this->size(), Qt::KeepAspectRatio);
            origin.setX((this->width() - spm.width()) / 2);
            origin.setY((this->height() - spm.height()) / 2);
            break;
        case ImageStretch:
            spm  = pxm.scaled(this->size());
            break;
        };
        painter.drawPixmap(origin, spm);

    }   
}

