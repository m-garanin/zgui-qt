#include "graphicsitem.h"
#include "IManager.h"

#include <QPainter>
#include <QFont>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#include <QDebug>

void myImageCleanupHandler(void *info){
    free(info);
}

CGraphicsItem::CGraphicsItem(qint32 compkey, QGraphicsItem *parent) :
    m_compkey(compkey),
    QGraphicsItem(parent),
    m_currentImage(NULL),
    m_imageFitMode(ImageStretch),
    _size(200,200),
    _edited(false)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);

    updatePreview();
}

QRectF CGraphicsItem::boundingRect() const
{
    return QRectF(QPointF(0,0), _size);
}

void CGraphicsItem::setEditMode(bool edited)
{
    _edited = edited;
    setAcceptDrops(_edited);
    setCursor(Qt::ArrowCursor);
}

bool CGraphicsItem::isEditMode() const
{
    return _edited;
}

void CGraphicsItem::setImageFitMode(CGraphicsItem::ImageFitMode mode)
{
    m_imageFitMode = mode;
    update();
}

void CGraphicsItem::updatePreview()
{
    if(m_compkey == 0)
        return;

    char* buf = NULL;
    int w,h;
    global_manager->getLastImage(m_compkey, &buf, &w, &h);
    QImage* pimg = new QImage((uchar*)buf, w, h, QImage::Format_RGB888, &myImageCleanupHandler, buf);
    drawImage(pimg);
}

void CGraphicsItem::drawImage(QImage *img)
{
    if(m_currentImage != NULL && m_currentImage != img)
        delete m_currentImage;
    m_currentImage = img;
}

void CGraphicsItem::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->fillRect(0,0, width(), height(), Qt::black);
    updatePreview();

    if(m_currentImage == NULL){
        painter->fillRect(0,0, width(), height(), Qt::black);
        painter->setPen(Qt::white);
        painter->setFont(QFont("Arial", 20));
        painter->drawText(rect(), Qt::AlignCenter, "wait...");

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
        painter->drawImage(origin, img);

        if(_edited)
        {
            QPen pen;
            pen.setColor(Qt::lightGray);
            painter->setPen(pen);

            int width = size().width();
            int height = size().height();
            painter->drawRect(0,0,width, height);

            painter->drawLine(width - 9, height, width, height - 9);
            painter->drawLine(width - 6, height, width, height - 6);
            painter->drawLine(width - 3, height, width, height - 3);
        }
        //painter->setRenderHint(QPainter::Antialiasing, false);
    }
}

QVariant CGraphicsItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    return QGraphicsItem::itemChange(change, value);
}

void CGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() != Qt::LeftButton)
        return;

    if(_edited)
        setCursor(Qt::ClosedHandCursor);
}

void CGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
}

void CGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() != Qt::LeftButton)
        return;

    if(_edited)
        setCursor(Qt::OpenHandCursor);
}






















