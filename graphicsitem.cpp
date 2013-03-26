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
    QGraphicsItem(parent),
    m_compkey(compkey),
    m_currentImage(NULL),
    m_imageFitMode(ImageStretch),
    _size(200,200),
    _edited(false),
    _isResizing(false)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setAcceptHoverEvents(true);

    updatePreview();
}

QRectF CGraphicsItem::boundingRect() const
{
    return QRectF(0, 0, _size.width(), _size.height());
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
    Q_UNUSED(option);
    Q_UNUSED(widget);

    updatePreview();

    int width = _size.width();
    int height = _size.height();

    painter->fillRect(0,0, width, height, Qt::black);

    if(m_currentImage == NULL)
    {
        painter->setPen(Qt::white);
        painter->setFont(QFont("Arial", 20));
        painter->drawText(boundingRect(), Qt::AlignCenter, "wait...");
    }
    else
    {
        QImage img;
        QPoint origin;
        switch (m_imageFitMode)
        {
            case ImageFit:
                img = m_currentImage->scaled(_size, Qt::KeepAspectRatio);
                origin.setX((width - img.width()) / 2);
                origin.setY((height - img.height()) / 2);
            break;
            case ImageStretch:
                img  = m_currentImage->scaled(_size);
            break;
        };
        painter->drawImage(origin, img);

        if(_edited)
        {
            painter->setRenderHint(QPainter::Antialiasing, true);

            QPen pen;
            pen.setColor(Qt::white);
            painter->setPen(pen);

            painter->drawRect(0,0,width, height);

            painter->drawLine(width - 13, height, width, height - 13);
            painter->drawLine(width - 9, height, width, height - 9);
            painter->drawLine(width - 5, height, width, height - 5);

            painter->setRenderHint(QPainter::Antialiasing, true);
        }
    }
}


void CGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (_edited && event->button() == Qt::LeftButton) {
        if(isInResizeArea(event->pos())) {
            _isResizing = true;
        } else {
            setCursor(Qt::ClosedHandCursor);
        }
    } else {
        QGraphicsItem::mousePressEvent(event);
    }

}

void CGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void CGraphicsItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(_edited) {
        if(isInResizeArea(event->pos())) {
            setCursor(Qt::SizeFDiagCursor);
        } else {
            setCursor(Qt::OpenHandCursor);
        }
    } else {
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsItem::hoverMoveEvent(event);
}

void CGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (_edited && event->button() == Qt::LeftButton) {
        if(_isResizing) {
            _isResizing = false;
        } else {
            setCursor(Qt::OpenHandCursor);
        }
    } else {
        QGraphicsItem::mouseReleaseEvent(event);
    }
}

bool CGraphicsItem::isInResizeArea(const QPointF &pos)
{
    return (pos.x() - boundingRect().width() + 15) > 0 && (pos.y() - boundingRect().height() + 15) > 0;
}






















