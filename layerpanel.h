#ifndef _LAYER_PANEL_H_
#define _LAYER_PANEL_H_

#include <QWidget>
#include <QTableWidget>

class CLayerWidget;

class CLayerPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CLayerPanel(QWidget *parent = 0);

    void addLayer(const QString &sourceName);

private:
    QTableWidget *_tableWidget;
    QList<CLayerWidget*> _listLayerWidgets;
};

#endif // _LAYER_PANEL_H_