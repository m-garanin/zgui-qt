#include "audiopanel.h"

#include <QHeaderView>
#include <QResizeEvent>


CAudioPanel::CAudioPanel(QWidget *parent) :
    QWidget(parent)
{
    _tableWidget = new QTableWidget(this);
    _tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _tableWidget->setShowGrid(true);
    _tableWidget->setColumnCount(1);
    _tableWidget->setRowCount(0);
    _tableWidget->horizontalHeader()->setVisible(false);
    _tableWidget->verticalHeader()->setVisible(false);
    _tableWidget->verticalHeader()->setDefaultSectionSize(50);
    _tableWidget->verticalHeader()->setMinimumSectionSize(50);
    _tableWidget->setFocusPolicy(Qt::NoFocus);
}

void CAudioPanel::addVolumeWidget(CVolumeWidget *volumeWidget)
{
    qint32 row = _tableWidget->rowCount();

    _tableWidget->setRowCount(row  + 1);

    _tableWidget->setIndexWidget(_tableWidget->model()->index(row, 0), volumeWidget);
}

void CAudioPanel::resizeEvent(QResizeEvent *resize)
{   
    _tableWidget->horizontalHeader()->setDefaultSectionSize(resize->size().width());
}
