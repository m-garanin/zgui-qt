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
    m_transparency = 0;
    //setAcceptDrops(true);
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
    QImage* pimg = new QImage((uchar*)buf, w, h, QImage::Format_ARGB32, &myImageCleanupHandler, buf);
    drawImage(pimg);
}

void PreviewWidget::drawImage(QImage *img)
{
    if(m_currentImage != NULL && m_currentImage != img)
        delete m_currentImage;
    m_currentImage = img;

    this->update();
}

void PreviewWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    qreal opacity = (100 - m_transparency) / 100.0;
    painter.setOpacity(opacity);

    painter.fillRect(0,0, width(), height(), Qt::lightGray); // background. TODO: может есть более правильный способ?

    if(m_currentImage == NULL){
        painter.fillRect(0,0, width(), height(), Qt::black);
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 20));
        painter.drawText(rect(), Qt::AlignCenter, "wait...");

    } else {
        QPixmap spm;
        QPixmap pxm;        
        pxm = QPixmap::fromImage(QImage(*m_currentImage));

        switch (m_imageFitMode) {
        case ImageFit:
            spm = pxm.scaled(this->size(), Qt::KeepAspectRatio);
            m_top_left.setX((this->width() - spm.width()) / 2);
            m_top_left.setY((this->height() - spm.height()) / 2);
            break;
        case ImageStretch:
            spm  = pxm.scaled(this->size());
            break;
        };

        m_img_size = spm.size();
        painter.drawPixmap(m_top_left, spm);

    }
}

void PreviewWidget::setTransparency(int value)
{
    if (value <= 100 && value >= 0) {
        m_transparency = value;
        update();
    }

}

