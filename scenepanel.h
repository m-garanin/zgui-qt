#ifndef _SCENE_PANEL_H_
#define _SCENE_PANEL_H_

#include <QWidget>
#include "graphicsview.h"

#include "scenewidget.h"
#include "layerwidget.h"

class CScenePanel : public QWidget
{
    Q_OBJECT
public:
    explicit CScenePanel(qint32 compkey, QWidget *parent = 0);

    void addCamLayer(const QString &sourceName);
    void addImageLayer(QString fname);
    void addSubSceneLayer();

    void start();
    void stop();


public slots:
    void onPbAddPreviewWidget();
    void onPbApply();

    void onEditLayer(qint32);
    void onUltimateShow();

private:
    //CSceneWidget *_sceneWidget;
    CGraphicsView *_graphicsView;
    QList<CLayerWidget*> _listLayerWidgets;

    CLayerWidget* addLayer(const QString &sourceName);
    void resizeEvent(QResizeEvent * event);
    void rePosition();
};

#endif // _SCENE_PANEL_H_
