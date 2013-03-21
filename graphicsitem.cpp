#include "graphicsitem.h"

#include <QPainter>
#include <QFont>
#include <QTimer>

#include <QDebug>
#include <QPixmap>
#include <QDrag>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#include <QMimeData>
#include <QBitmap>
#include <QMenu>
#include <QAction>

#include "IManager.h"

extern void myImageCleanupHandler(void *info);

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

    start();
}

QRectF CGraphicsItem::boundingRect() const
{
    return QRectF(QPointF(0,0),_size);
}

void CGraphicsItem::start()
{
    // заводим таймер
    updatePreview();
}

void CGraphicsItem::stop()
{
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

    this->update();
}


QImage CGraphicsItem::image()
{
    return QImage();//QPixmap::grabWidget(this).toImage();
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

void CGraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    QAction *zOrderUp = menu.addAction("Up");
    QAction *zOrderDown = menu.addAction("Down");

//    QMenu *menuOrder = menu.addMenu("Opacity");
//    for(int i = 10; i <= 100; i+=10)
//    {
//        QAction * action = menuOrder->addAction(QString("%1").arg(i));
//        action->setProperty("opacity", i);
//    }

    QAction *selectedAction = menu.exec(event->screenPos());

    if(selectedAction == zOrderUp)
    {
        setZValue(zValue() + 0.001);
        qDebug() << "ZOreder: " << zValue();
    }
    else if(selectedAction == zOrderDown)
    {
        setZValue(zValue() - 0.001);
        qDebug() << "ZOreder: " << zValue();
    }
//    else
//    {
//        if(selectedAction != 0)
//        {
//            qreal transparent = selectedAction->property("opacity").toReal();
//            setOpacity(transparent/qreal(100.0));
//            qDebug() << "opacity: " << transparent/qreal(100.0);
//        }
//    }
    update();
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






















