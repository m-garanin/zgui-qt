#ifndef BOXWIDGET_H
#define BOXWIDGET_H

#include "previewwidget.h"
class QMenu;

class CBoxWidget : public PreviewWidget
{
    Q_OBJECT
public:
    explicit CBoxWidget(qint32 compkey, QWidget *parent = 0);
    void show();
    void hide();
    int transparency() const;
    void setTransparency(int value);

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


    QPoint m_dragPosition;
    WindowState m_windowState;
    bool m_dragging;
    bool m_keepAspectRatio;
    QMenu *m_contextMenu;
//    QPoint m_prevPos;

};

#endif // BOXWIDGET_H
