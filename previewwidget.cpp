#include "previewwidget.h"
#include <QPainter>
#include <QFont>
#include <QTimer>

#include <QDebug>

#include "IManager.h"

void myImageCleanupHandler(void *info){
    free(info);
}

PreviewWidget::PreviewWidget(QWidget *parent) :
    QWidget(parent),m_currentImage(NULL), m_imageFitMode(ImageFit)
{
    m_compkey = 0;
    _resizeBegin = false;

    setAcceptDrops(true);
}

void PreviewWidget::start(int compkey, int ms)
{
    this->m_compkey = compkey;
    // заводим таймер
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePreview()));
    timer->start(ms);
}

void PreviewWidget::setImageFitMode(PreviewWidget::ImageFitMode mode)
{
    m_imageFitMode = mode;
    update();
}

void PreviewWidget::updatePreview()
{
#ifdef Q_OS_WIN32
    if(m_compkey == 0)
        return;
    char* buf = NULL;
    int w,h;
    global_manager->getLastImage(m_compkey, &buf, &w, &h);
    QImage* pimg = new QImage((uchar*)buf, w, h, QImage::Format_RGB888, &myImageCleanupHandler, buf);
    drawImage(pimg);
#endif
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
    return *m_currentImage;
}

void PreviewWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.save();
    if(m_currentImage == NULL){
        painter.fillRect(0,0, width(), height(), Qt::red);
        painter.setPen(Qt::blue);
        painter.setFont(QFont("Arial", 30));
        painter.drawText(rect(), Qt::AlignCenter, "NULL");

    } else {
        QImage img;
        QPoint origin;
        switch (m_imageFitMode) {
        case ImageFit:
        {
            QSize s = m_currentImage->size();
            if (s.width() > s.height() || s.width() == s.height()) {
                img = m_currentImage->scaledToWidth(this->width());
            } else if (s.width() < s.height()) {
                img = m_currentImage->scaledToHeight(this->height());
            }
            origin.setX((this->width() - img.width()) / 2);
            origin.setY((this->height() - img.height()) / 2);
        }
            break;
        case ImageStretch:
            img  = m_currentImage->scaled(this->size());
            break;
        };
        painter.drawImage(origin, img);
    }

    QPen pen;
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.drawRect(size().width() - 10, size().height() - 10, 9, 9);
    painter.restore();
}

bool PreviewWidget::event(QEvent *event)
{
    if(event->type() == QEvent::MouseMove)
    {
        qDebug() << "das";
        if(_resizeBegin)
        {
            QPoint cursor_pos = mapFromGlobal(QCursor::pos());
            resize(cursor_pos.x(), cursor_pos.y());
            return true;
        }
    }
    if(event->type() == QEvent::MouseButtonPress)
    {
        QPoint cursor_pos = mapFromGlobal(QCursor::pos());

        if((size().width() - 10) < cursor_pos.x() && (size().height() - 10) < cursor_pos.y())
        {
            _resizeBegin = true;
            return true;
        }
    }
    return QWidget::event(event);
}

void PreviewWidget::enterEvent(QEvent *event)
{
    qDebug() << QCursor::pos().x();
    QWidget::enterEvent(event);
}

void PreviewWidget::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
}

//void PreviewWidget::mousePressEvent(QMouseEvent *event)
//{

//}

void PreviewWidget::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "x: " << event->pos().x() << " y: " << event->pos().y();

    QWidget::mouseMoveEvent(event);
}

void PreviewWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    _resizeBegin = false;
    QWidget::mouseReleaseEvent(event);
}
