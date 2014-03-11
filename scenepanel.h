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

    CLayerWidget *addCamLayer(const QString &sourceName, QString psize);
    CLayerWidget *addImageLayer(QString fname);
    CLayerWidget* addHtmlPluginLayer(QString fname);
    CLayerWidget *addSubSceneLayer();
    CLayerWidget *addScreenCaptureLayer(QRect rect);
    CLayerWidget *addVideoFileLayer(QString fname);
    CLayerWidget *addNetSourceLayer(QString uri);
    CLayerWidget *addWebPageLayer(QString uri);

    CLayerWidget* findLayerWidgetByCompkey(qint32 compkey);

    void start();
    void stop();

    void applySetting();

    //void hideLayers(); BBB

    QJsonObject saveState();
    void restoreState(QJsonObject mobj);

    void saveStateToFile(QString fname);
    void restoreStateFromFile(QString fname);

public slots:    
    void onEditLayer(qint32);
    void onUltimateShow();

    void onImageSelect();
    void onVideoCaptureSelect();
    void onAddHtmlRender();
    void onVideoFileSelect();

    void onAddScreenCapture();
    void onScreenCaptureSelected();
    void onNetSourceSelect();
    void onWebPageSelect();
    void onDeleteLayer();

    void onSaveState();
    void onRestoreState();

    void onAddPlayback();

private:
    qint32 _compkey;
    CSceneWidget *_sceneWidget;
    QList<CLayerWidget*> _listLayerWidgets;
    int m_external_count;

    CLayerWidget* addLayer(const QString &type, const QString &sourceName, CLayerWidget::LayerType lType=CLayerWidget::ELayerTypeDefault, QString strinfo=NULL);
    void resizeEvent(QResizeEvent * event);
    void rePosition();

    void restoreLayer(QJsonObject obj);

    bool is_main() {return _compkey == 100 ;} // признак что это основная панель
};

#endif // _SCENE_PANEL_H_
