#ifndef _SCENE_PANEL_H_
#define _SCENE_PANEL_H_

#include <QWidget>
#include "scenewidget.h"

#include "scenewidget.h"
#include "layerwidget.h"

class CScenePanel : public QWidget
{
    Q_OBJECT
public:
    explicit CScenePanel(qint32 compkey, QWidget *parent = 0);

    void addCamLayer(const QString &sourceName);
    void addImageLayer(QString fname);
    void addHtmlRenderLayer(const QString &url);
    void addSubSceneLayer();

    void start();
    void stop();

    void applySetting();

public slots:
    void onPbAddPreviewWidget();
    void onPbApply();

    void onEditLayer(qint32);
    void onUltimateShow();

    void onShowSceneWidget();

private:
    qint32 _compkey;
    CSceneWidget *_sceneWidget;
    QList<CLayerWidget*> _listLayerWidgets;

    CLayerWidget* addLayer(const QString &sourceName);
    void resizeEvent(QResizeEvent * event);
    void rePosition();
};

#endif // _SCENE_PANEL_H_
