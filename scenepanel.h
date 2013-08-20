#ifndef _SCENE_PANEL_H_
#define _SCENE_PANEL_H_

#include <QWidget>
#include "scenewidget.h"

#include "scenewidget.h"
#include "layerwidget.h"
#include "rectselectionwidget.h"

class CScenePanel : public QWidget
{
    Q_OBJECT
public:
    explicit CScenePanel(qint32 compkey, QWidget *parent = 0);

    void addCamLayer(const QString &sourceName);
    void addImageLayer(QString fname);
    //void addHtmlRenderLayer();
    void addSubSceneLayer();
    void addScreenCaptureLayer(RectSelectionWidget *w);


    CLayerWidget* findLayerWidgetByCompkey(qint32 compkey);

    void start();
    void stop();

    void applySetting();

    void hideLayers();

public slots:    
    void onEditLayer(qint32);
    void onUltimateShow();

    void onImageSelect();
    void onVideoCaptureSelect();
    void onAddHtmlRender();

    void onAddScreenCapture();
    void onScreenCaptureSelected();

private:
    qint32 _compkey;
    CSceneWidget *_sceneWidget;
    QList<CLayerWidget*> _listLayerWidgets;
    int m_external_count;
    CLayerWidget* addLayer(const QString &type, const QString &sourceName);
    void resizeEvent(QResizeEvent * event);
    void rePosition();
};

#endif // _SCENE_PANEL_H_
