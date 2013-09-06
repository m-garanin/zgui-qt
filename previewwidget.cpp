#include "previewwidget.h"
#include <QPainter>
#include <QFont>
#include <QTimer>

#include <QDebug>
#include <QPixmap>

#include "IManager.h"


PreviewWidget::PreviewWidget(qint32 compkey, bool need_quality, QWidget *parent) :
    m_compkey(compkey),
    m_need_quality(need_quality),
    m_orig_size(0,0),
    m_rec(0,0,0,0),
    m_prv_num(0),
    QWidget(parent),m_currentImage(NULL), m_imageFitMode(ImageFit)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePreview()));
    start();
    m_transparency = 0;    

    setObjectName("Preview");
    setAttribute(Qt::WA_StyledBackground, true);

    m_temp_count = 0;
}

void PreviewWidget::start()
{
    // заводим таймер
    if(!timer->isActive())
    {        
        timer->start(33); // 33 соответствует 30 FPS
    }
}

void PreviewWidget::stop()
{
    timer->stop();
    m_currentImage = NULL;
}


void PreviewWidget::setImageFitMode(PreviewWidget::ImageFitMode mode)
{
    m_imageFitMode = mode;
    recalcPosition();
    update();
}

void PreviewWidget::updatePreview()
{
    if(m_compkey == 0)
        return;

    if( m_temp_count < 25*4){
        m_temp_count ++;
        //return;
    }

    char* buf = NULL;
    int size, w,h;

    global_manager->getPreview(m_compkey, &buf, &size, &w, &h, &m_prv_num );

    if(buf == NULL){
        //qDebug() << "BUF NULL";
        return;
    }
    //qDebug() << "size" << w << "x" << h;

    QImage* pimg = new QImage((uchar*)buf, w, h, QImage::Format_ARGB32);
    if( m_orig_size.width() != w || m_orig_size.height() != h ){
        m_orig_size = QSize(w,h);
        recalcPosition();
    }

    drawImage(pimg); // TODO: встроить
    global_manager->unrefPreview(m_compkey);

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

    //painter.fillRect(0,0, width(), height(), Qt::green ); // background. TODO: может есть более правильный способ?

    if(m_currentImage == NULL){
        painter.fillRect(0,0, width(), height(), Qt::black);
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 20));
        painter.drawText(rect(), Qt::AlignCenter, "wait...");
        return;
    }

    if(m_need_quality){
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setRenderHint(QPainter::TextAntialiasing, true);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    }

    painter.drawImage(m_rec, *m_currentImage);
}

void PreviewWidget::resizeEvent(QResizeEvent *event)
{
    recalcPosition();
}

void PreviewWidget::setTransparency(int value)
{
    if (value <= 100 && value >= 0) {
        m_transparency = value;
        update();
    }

}

void PreviewWidget::recalcPosition()
{
    // результатом является подсчитанные m_rec и m_img_size, которые зависят
    // 1) от размеров картинки (m_orig_size)
    // 2) от размеров виджета
    // 3) от требуемого режима отображения

    // если режим ImageStretch
    if(m_imageFitMode == ImageStretch){
        m_img_size = this->size();
        m_rec = this->rect();
        return;
    }

    // для режима ImageFit
    int width, height, w, h, x, y;
    double r;

    width = this->width();
    height = this->height();
    if( m_orig_size.height() == 0){
        //qDebug() << "FUCK";
        r = 1;
    }else{
        r = (double)m_orig_size.width() / m_orig_size.height();
    }
    if(r == 0)  r = 1;
    // смотрим первый вариант (максимальная ширина)
    w = width;
    h = w/r;
    x = 0;
    y = (height - h) / 2;
    // если высота при этом вылезает из зоны, значит нужно брать другой вариант
    if(h > height){
        h = height;
        w = h * r;
        x = (width - w) / 2;
        y = 0;
    }

    m_rec = QRect(x,y, w,h);
    m_img_size = QSize(w,h);

}

