#ifndef _SCENE_PANEL_H_
#define _SCENE_PANEL_H_

#include "scenewidget.h"
#include "layerpanel.h"

#include <QWidget>

class CScenePanel : public QWidget
{
    Q_OBJECT
public:
    explicit CScenePanel(QWidget *parent = 0);

    void addSource();

public slots:
    void onPbAddPreviewWidget();
    void onPbApply();

private:
    CSceneWidget *_sceneWidget;
    CLayerPanel *_layerPanel;

};

#endif // _SCENE_PANEL_H_