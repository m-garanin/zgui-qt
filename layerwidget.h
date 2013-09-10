#ifndef LAYERWIDGET2_H
#define LAYERWIDGET2_H

#include <QLineEdit>
#include <QMenu>
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
        ELayerTypeCAM,
        ELayerTypeDefault
    };

    explicit CLayerWidget(qint32 compkey, LayerType type = ELayerTypeDefault, QWidget *parent = 0);

    qint32 compKey() const;

    void setVisibleState(bool visible);
    bool isVisibleHide() const;

    bool isPinEnable() const;

    void start();
    void stop();



    //void setEnabledOpenGl(bool enable);

//protected:
   // void timerEvent(QTimerEvent *event);
    //void resizeEvent(QResizeEvent *event);

public slots:
    void setTitle(QString txt);

    void onPbResizeClicked();
    void onPbEffectClicked();    
    void onPbConstructClicked();

    void onPbVisibleClicked();
    void onPbPinToggled(bool);
    void onNextImage();
    void onPrevImage();
    void onHTMLPluginSettings();

signals:
    void showSignal();
    void editLayer(qint32);
    void ultimateShow();
    void switchImage(bool);
    void openHTMLPluginSettings();

private:
    qint32 _compkey;
    bool _pin;
    bool _is_visible;
    LayerType _layerType;

    PreviewWidget *_previewWidget;
    CLayerConstructDlg *_layerConstructDlg;
    QPushButton *_pbVisibleHide;
    QPushButton *_pbPin;

    QLineEdit* _title;
    qint32 _timerId;

    QMenu *m_contextMenu;

    void contextMenuEvent(QContextMenuEvent *event);

};

#endif // LAYERWIDGET2_H
