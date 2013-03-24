#ifndef LAYERWIDGET2_H
#define LAYERWIDGET2_H

#include <QGraphicsView>

QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(CLayerConstructDlg)
QT_FORWARD_DECLARE_CLASS(CGraphicsItem)

class CLayerWidget : public QGraphicsView
{
    Q_OBJECT
public:
    enum LayerType
    {
        ELayerTypeSUBSCENE,
        ELayerTypeIMAGE,
        ELayerTypeCAM,
        ELayerTypeDefault
    };

    explicit CLayerWidget(qint32 compkey, LayerType type = ELayerTypeDefault, QWidget *parent = 0);

    qint32 compKey() const;

    void setVisibleHide(bool);
    bool isVisibleHide() const;

    bool isPinEnable() const;

    void start();
    void stop();

protected:
    void timerEvent(QTimerEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    void initBtn();

public slots:
    void onPbResizeClicked();
    void onPbEffectClicked();
    void onPbUltimateShowClicked();
    void onPbConstructClicked();

    void onPbVisibleHideToggled(bool);
    void onPbPinToggled(bool);

signals:
    void editLayer(qint32);
    void ultimateShow();

private:
    qint32 _compkey;
    bool _pin;
    LayerType _layerType;

    CLayerConstructDlg *_layerConstructDlg;
    QPushButton *_pbVisibleHide;

    qint32 _timerId;
};

#endif // LAYERWIDGET2_H
