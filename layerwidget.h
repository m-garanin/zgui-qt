#ifndef _LAYERWIDGET_H_
#define _LAYERWIDGET_H_

#include <QWidget>

class CLayerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CLayerWidget(qint32 compkey, QWidget *parent = 0);

public slots:
    void onPbVisibleHideClicked();
    void onPbResizeClicked();
    void onPbEffectClicked();
    void onPbPinClicked();
    void onPbUltimateShowClicked();

signals:
    void resize(qint32);

private:
    qint32 _compkey;

};

#endif //_LAYERWIDGET_H_