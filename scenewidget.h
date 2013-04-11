#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include "previewwidget.h"

class QPaintEvent;
class CBoxWidget;
class QMenu;
class QDropEvent;
class QAction;
class QScrollArea;

class CSceneWidget : public PreviewWidget
{
    Q_OBJECT
public:
    explicit CSceneWidget(qint32 compkey, QWidget *parent = 0);

    void toggleBox(int compkey); // показывает либо скрывает бокс
    void apply();

    void startBox();
    void stopBox();
    // overrided (non-virtual) method. maybe it's better to rename it;
    void setGeometry(int x, int y, int width, int height);
    void zoomIn();
    void zoomOut();
    void setZoomFactor(qreal zoomFactor);

public slots:
    void setGridVisible(bool visible);
    void setCellWidth(quint32);

private slots:
    void onCustomContextMenuRequested(const QPoint &);
    void onApplyTriggered();
    void onHideBoxTriggerd();
    void onCloneTriggered();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    void hideBoxes();
    void drawGrid();
    void zoom(qreal zoomFactor);

private:
    QList<CBoxWidget*> _boxWidgetList;
    QScrollArea  * m_sa;
    QMenu *_menu;
    quint32 m_cellWidth;
    bool    m_gridEnabled;
    qreal   m_zoomFactor;
    QSize   m_initialSize;

};

#endif // SCENEWIDGET_H
