#include "scenepanel.h"

#include <QLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QDebug>

CScenePanel::CScenePanel(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(6);
    mainLayout->setContentsMargins(11, 11, 11, 11);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    QGridLayout *gridLayoutForSceneWidget = new QGridLayout();
    gridLayoutForSceneWidget->setSpacing(6);
    _sceneWidget = new CSceneWidget(this);
    
    //gridLayoutForSceneWidget->addWidget(_sceneWidget, 0, 0, 1, 1);
    //mainLayout->addLayout(gridLayoutForSceneWidget);

    gridLayoutForSceneWidget->addWidget(_sceneWidget, 0, 0, 1, 3);
    QPushButton *pbAddPreviewWidget = new QPushButton("Add Preview Widget", this);
    connect(pbAddPreviewWidget, SIGNAL(clicked()), SLOT(onPbAddPreviewWidget()));
    gridLayoutForSceneWidget->addWidget(pbAddPreviewWidget, 1, 2, 1, 1);
    
    QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayoutForSceneWidget->addItem(horizontalSpacer, 1, 0, 1, 1);

    QPushButton *pbApply = new QPushButton("Apply", this);
    connect(pbApply, SIGNAL(clicked()), SLOT(onPbApply()));
    gridLayoutForSceneWidget->addWidget(pbApply, 1, 1, 1, 1);

    mainLayout->addLayout(gridLayoutForSceneWidget);

    QGridLayout *gridLayoutForLayerPanel = new QGridLayout();
    gridLayoutForLayerPanel->setSpacing(6);
    _layerPanel = new CLayerPanel();

    gridLayoutForLayerPanel->addWidget(_layerPanel, 0, 0, 1, 1);    
    mainLayout->addLayout(gridLayoutForLayerPanel);
    mainLayout->setStretch(0, 1);
    mainLayout->setStretch(1, 1);
}

void CScenePanel::addSource()
{
    _layerPanel->addLayer("UNUSED");
}

void CScenePanel::onPbAddPreviewWidget()
{
    _sceneWidget->showBox(1);
}

void CScenePanel::onPbApply()
{
    qDebug() << _sceneWidget->apply();
}