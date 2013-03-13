#ifndef _SCENE_PANEL_H_
#define _SCENE_PANEL_H_

#include <QWidget>

#include "scenewidget.h"
#include "layerwidget.h"

class CScenePanel : public QWidget
{
    Q_OBJECT
public:
    explicit CScenePanel(qint32 compkey, QWidget *parent = 0);

    void addLayer(const QString &sourceName);

public slots:
    void onPbAddPreviewWidget();
    void onPbApply();

private:
    CSceneWidget *_sceneWidget;    
    QList<CLayerWidget*> _listLayerWidgets;

    void resizeEvent(QResizeEvent * event);
    void rePosition();
};

#endif // _SCENE_PANEL_H_
