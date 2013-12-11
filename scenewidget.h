#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QPushButton>
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
    explicit CSceneWidget(qint32 compkey, bool is_clone=false, QWidget *parent = 0);

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
    void onEffectsTriggered();

    void onFullScreenTriggered(bool check);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    void hideBoxes();
    void drawGrid();
    void zoom(qreal zoomFactor);
    void setButtonBar();

    void enableBoxButtons(bool flag);
    void checkConstructMode();

private:
    QList<CBoxWidget*> _boxWidgetList;
    QScrollArea  * m_sa;
    void*   m_Panel;
    QMenu *_menu;
    quint32 m_cellWidth;
    bool    m_gridEnabled;
    qreal   m_zoomFactor;
    QSize   m_areaSize; // размер области в которой отображается виджет

    QPushButton* m_pbClean;
    QPushButton* m_pbApply;
};

#endif // SCENEWIDGET_H
