#ifndef _LAYERWIDGET_H_
#define _LAYERWIDGET_H_

#include <QWidget>

class QPushButton;

class CLayerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CLayerWidget(qint32 compkey, QWidget *parent = 0);
    
    qint32 compKey() const;

    void setVisibleHide(bool);
    bool isVisibleHide() const;

    bool isPinEnable() const;

public slots:
    void onPbResizeClicked();
    void onPbEffectClicked();
    void onPbUltimateShowClicked();

    void onPbVisibleHideToggled(bool);
    void onPbPinToggled(bool);

signals:
    void editLayer(qint32);
    void ultimateShow();

private:
    qint32 _compkey;
    bool _pin;

    QPushButton *_pbVisibleHide;
};

#endif //_LAYERWIDGET_H_