#ifndef RECTSELECTIONWIDGET_H
#define RECTSELECTIONWIDGET_H

#include <QWidget>
#include <QRect>

class QPoint;
class RectSelectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RectSelectionWidget(QWidget *parent = 0);

    QRect grab_geometry();
signals:
    void cancelled();
    void submitted();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private slots:
    void onCancelTriggered();

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

#endif // RECTSELECTIONWIDGET_H
