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

    int getCompkey() { return m_compkey; }

    void setSize(const QSize &size) { _size = size; }
    QSize imageSize() const;

    QRectF boundingRect() const;
protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool isInResizeArea(const QPointF &);

public slots:
    void updatePreview();

private:
    int m_compkey;
    QImage* m_currentImage;
    ImageFitMode m_imageFitMode;
    QSize _size;
    bool _edited;
    bool _isResizing;
};


#endif // GRAPHICSITEM_H
