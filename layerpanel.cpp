#include "layerpanel.h"

#include "layerwidget.h"

#include <QHeaderView>
#include <QLayout>
#include <QLabel>
#include <QDebug>

CLayerPanel::CLayerPanel(QWidget *parent) :
    QWidget(parent)
{   
    _tableWidget = new QTableWidget(this);
    _tableWidget->setGeometry(0, 0, geometry().height(), geometry().width()); // TODO: HACK!
    _tableWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    _tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _tableWidget->setShowGrid(true);
    _tableWidget->setRowCount(2);
    _tableWidget->setColumnCount(3);
    _tableWidget->horizontalHeader()->setVisible(false);
    _tableWidget->horizontalHeader()->setDefaultSectionSize(150);
    _tableWidget->verticalHeader()->setVisible(false);
    _tableWidget->verticalHeader()->setDefaultSectionSize(150);
    _tableWidget->verticalHeader()->setMinimumSectionSize(150);
    _tableWidget->setFocusPolicy(Qt::NoFocus);
}

void CLayerPanel::addLayer(const QString &sourceName)
{
    // TODO: create N CLayerWidget
    if (_listLayerWidgets.count() >= 6)
    {
        qDebug() << "Cam max 6 element.";
        return;
    }
    CLayerWidget *lw = new CLayerWidget(100, this);
    _listLayerWidgets.push_back(lw);

    qint32 column_count = 3;
    qint32 count = _listLayerWidgets.count() - 1;

    int row = count/column_count;
    int col = count%column_count;

    _tableWidget->setIndexWidget(_tableWidget->model()->index(row, col), _listLayerWidgets.last());
}
