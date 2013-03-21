#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include "graphicsitem.h"

#include <QGraphicsView>
#include <QMenu>

class CGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    CGraphicsView(qint32 compkey, QWidget *parent = 0);

    void showBox(qint32);

    qint32 getCompkey() const;

    QStringList apply();

    void startBox();
    void stopBox();

    void start(); // запускает процесс обновления
    void stop();

public slots:
    void onZoomIn();
    void onZoomOut();

    void onApplyTriggered();
    void onHideBoxTriggerd(bool);
    void setGridVisible(bool);
    void onCloneTriggered();

protected:
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void drawForeground(QPainter *painter, const QRectF &rect);
    void paintEvent(QPaintEvent *event);

    void mouseMoveEvent ( QMouseEvent * event );
    void mousePressEvent ( QMouseEvent * event );
    void mouseReleaseEvent ( QMouseEvent * event );
    void wheelEvent(QWheelEvent *event);

    void contextMenuEvent(QContextMenuEvent *);

private:
    void scaleView(qreal scaleFactor);
    void initMenu();
    void drawGrid(QPainter *);
    void setCellWidth(quint32 arg);

private:
    qint32 _compkey;
    QGraphicsScene *_scene;
    CGraphicsItem *_currentItem;
    QPointF _offsetMove;
    QImage* m_currentImage;
    bool _resizeBegin;
    bool m_gridEnabled;
    quint32 m_cellWidth;
    qint32 _timerId;

    qint32 posx;
    QMenu *_menu;
};
#endif // GRAPHICSVIEW_H
