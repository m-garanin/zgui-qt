#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QPushButton>
#include "previewwidget.h"
#include "mixer.h"
#include "scene.h"

class QPaintEvent;
class CBoxWidget;
class QMenu;
class QDropEvent;
class QAction;
class QScrollArea;

class CSceneWidget : public PreviewWidget, public Mixer
{
    Q_OBJECT
public:
    explicit CSceneWidget(Scene* ps, bool is_clone=false, QWidget *parent = 0);

    void toggleBox(Layer *pl); // показывает либо скрывает бокс
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
    void showFotoIcon();

private slots:
    void onCustomContextMenuRequested(const QPoint &);
    void onApplyTriggered();
    void onHideBoxTriggerd();
    void onCloneTriggered();
    void onEffectsTriggered();
    void onFotoTriggered();

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
    Scene* m_scene;
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
    QPushButton* m_pbFoto;
};

#endif // SCENEWIDGET_H
