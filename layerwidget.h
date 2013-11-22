#ifndef LAYERWIDGET2_H
#define LAYERWIDGET2_H

#include <QLineEdit>
#include <QMenu>
#include <QSlider>
#include <QJsonObject>
#include <QTimer>
#include <QLayout>
#include "IManager.h"
#include "previewwidget.h"

QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(CLayerConstructDlg)
QT_FORWARD_DECLARE_CLASS(PreviewWidget)


class CLayerWidget : public QWidget
{
    Q_OBJECT
public:
    enum LayerType
    {
        ELayerTypeHTMLPLUGIN,
        ELayerTypeSUBSCENE,
        ELayerTypeIMAGE,
        ELayerTypeVIDEO,
        ELayerTypeCAM,
        ELayerTypeSCREEN,
        ELayerTypeDefault
    };
    enum LayerMode{
        NormalMode,
        BkgMode,
        OvrMode
    };

    explicit CLayerWidget(qint32 compkey, LayerType type = ELayerTypeDefault, QWidget *parent = 0);

    qint32 compKey() const;

    void setVisibleState(bool visible);
    bool isVisibleHide() const;

    bool isPinEnable() const;

    void start();
    void stop();

    void setZOrder(int zorder);

    LayerMode getLayerMode() {return m_mode;}
    LayerType getLayerType() {return _layerType;}

    void setPersistentSourceId(QString name) {m_persistent_source_id = name; }

    // for state storage
    QString getPersistentSourceId(){return m_persistent_source_id;}
    QString getTitle() {return _title->text();}

    QString typeAsString();
    QString modeAsString();

    //int getZOrder(){return m_zorder;} // TODO

    void* getProxyScenePanel();

public slots:
    void setTitle(QString txt);

    void onPbResizeClicked();
    void onPbEffectClicked();    
    void onPbConstructClicked();

    void onPbVisibleClicked();
    //void onPbPinToggled(bool);
    void onNextImage();
    void onPrevImage();
    void onHTMLPluginSettings();


    void onSetOptimalPos();
    void onSetFullPos();

    void onSetBkgMode();
    void onSetOvrMode();
    void setMode(LayerMode md=LayerMode::NormalMode);

    void onMoveUp();
    void onMoveDown();

    void onDelete();

    // Playback
    void onPlayClicked();
    void onPlaybackSliderClick();
    void onPlaybackTimer();

signals:
    void showSignal();
    void editLayer(qint32);
    void ultimateShow();
    void switchImage(bool);
    void openHTMLPluginSettings();
    void deleteLayer();

private:
    qint32 _compkey;
    bool _pin;
    bool _is_visible;
    int m_zorder;
    LayerType _layerType;
    LayerMode m_mode;

    PreviewWidget *_previewWidget;
    CLayerConstructDlg *_layerConstructDlg;
    QPushButton *_pbVisibleHide;
    QPushButton * m_pbMode;

    QLineEdit* _title;
    qint32 _timerId; // TODO: выяснить что и зачем
    QString m_persistent_source_id;
    QMenu *m_contextMenu;

    void contextMenuEvent(QContextMenuEvent *event);

    // playback
    QPushButton * m_pbPlay;
    bool m_playback_play;
    IPlaybackSource* m_playback;
    QSlider* m_playback_slider;
    QTimer *m_playback_timer;


};

#endif // LAYERWIDGET2_H
