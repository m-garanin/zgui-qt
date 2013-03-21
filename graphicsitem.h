#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

#include <QGraphicsItem>

class CGraphicsItem : public QGraphicsItem
{
public:
    enum ImageFitMode {
        ImageFit,
        ImageStretch
    };

    explicit CGraphicsItem(qint32 compkey, QGraphicsItem *parent = 0);
    void drawImage(QImage* img);
    ImageFitMode imageFitMode() const { return m_imageFitMode; }
    void setImageFitMode(ImageFitMode mode);

    void setEditMode(bool);
    bool isEditMode() const;

    int getCompkey() {return m_compkey;}

    qreal width() {return _size.width();}
    qreal height() {return _size.height();}

    void setSize(const QSize &size) {_size = size;}
    QSize size() {return _size;}
    QRect rect() {return QRect(QPoint(pos().x(),pos().y()), _size);}
    QRectF boundingRect() const;
protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

public slots:
    void updatePreview();

private:
    int m_compkey;
    QImage* m_currentImage;
    ImageFitMode m_imageFitMode;
    QSize _size;
    bool _edited;
};


#endif // GRAPHICSITEM_H
