#ifndef BOXWIDGET_H
#define BOXWIDGET_H

#include "previewwidget.h"

class CBoxWidget : public PreviewWidget
{
    Q_OBJECT
public:
    explicit CBoxWidget(qint32 compkey, QWidget *parent = 0);
    
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

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

    QPoint m_dragPosition;
    WindowState m_windowState;
    bool m_dragging;

};

#endif // BOXWIDGET_H
