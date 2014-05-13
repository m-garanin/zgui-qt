#ifndef BOXWIDGET_H
#define BOXWIDGET_H

#include "previewwidget.h"
#include "layer.h"

class QMenu;

class CBoxWidget : public PreviewWidget
{
    Q_OBJECT
public:
    explicit CBoxWidget(Layer* pl, QWidget *parent = 0);
    void show();
    void hide();
    int transparency() const;
    void setTransparency(int value);

    Layer* layer() {return m_layer;}

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void contextMenuEvent(QContextMenuEvent *);

private slots:
    void zOrderMoveUp();
    void zOrderMoveDown();
    void toggleAspectRatio(bool);
    void onTransparencyMenuChanged(QAction *);
    void onHideTriggered();


private:
    enum WindowState {
        Idle,
        Dragging,
        ResizingLeft,
        ResizingTopLeft,
        ResizingTop,
        ResizingTopRight,
        ResizingRight,
        ResizingBottomRight,
        ResizingBottom,
        ResizingBottonLeft
    };

    WindowState windowState(const QPoint &pt);
    void updateCursor(WindowState state);

    Layer* m_layer;
    QPoint m_dragPosition;
    WindowState m_windowState;
    bool m_dragging;
    bool m_keepAspectRatio;
    QMenu *m_contextMenu;
    QPoint m_prevPos;
    QSize  m_prevSize;

};

#endif // BOXWIDGET_H
