#include "layerwidget.h"
#include "previewwidget.h"

#include <QLayout>
#include <QPushbutton>
#include <QDebug>

CLayerWidget::CLayerWidget(int compkey, QWidget *parent) :
    _compkey(compkey),
    QWidget(parent)
{
    QHBoxLayout *horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(11, 11, 11, 11);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(6);
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    verticalLayout->addItem(verticalSpacer);

    QPushButton *pbVisibleHide = new QPushButton("V", this);
    pbVisibleHide->setMaximumSize(QSize(20, 16777215));
    pbVisibleHide->setToolTip("visible/hide");
    verticalLayout->addWidget(pbVisibleHide);
    connect(pbVisibleHide, SIGNAL(clicked()), SLOT(onPbVisibleHideClicked()));

    QPushButton *pbResize = new QPushButton("R", this);
    pbResize->setMaximumSize(QSize(20, 16777215));
    pbResize->setToolTip("resize");
    verticalLayout->addWidget(pbResize);
    connect(pbResize, SIGNAL(clicked()), SLOT(onPbResizeClicked()));

    QPushButton *pbEffect = new QPushButton("E", this);
    pbEffect->setMaximumSize(QSize(20, 16777215));
    pbEffect->setToolTip("effect");
    verticalLayout->addWidget(pbEffect);
    connect(pbEffect, SIGNAL(clicked()), SLOT(onPbEffectClicked()));

    QPushButton *pbPin = new QPushButton("P", this);
    pbPin->setMaximumSize(QSize(20, 16777215));
    pbPin->setToolTip("pin");
    verticalLayout->addWidget(pbPin);
    connect(pbPin, SIGNAL(clicked()), SLOT(onPbPinClicked()));

    QPushButton *pbUltimateShow = new QPushButton("U", this);
    pbUltimateShow->setMaximumSize(QSize(20, 16777215));
    pbUltimateShow->setToolTip("ultimate show");
    verticalLayout->addWidget(pbUltimateShow);
    connect(pbUltimateShow, SIGNAL(clicked()), SLOT(onPbUltimateShowClicked()));
        
    PreviewWidget *pw = new PreviewWidget(compkey, this);
    //pw->setGeometry(0,0,40,40);
    //pw->start(compkey, 40); // TODO: we must start widget on create?
    
    horizontalLayout->addLayout(verticalLayout);
    horizontalLayout->addWidget(pw);

    horizontalLayout->setStretch(1, 4);
}

void CLayerWidget::onPbVisibleHideClicked()
{
    if(QPushButton *pb = qobject_cast<QPushButton*>(sender()))
        qDebug() << pb->toolTip();
}

void CLayerWidget::onPbResizeClicked()
{
    if(QPushButton *pb = qobject_cast<QPushButton*>(sender()))
        qDebug() << pb->toolTip();
}

void CLayerWidget::onPbEffectClicked()
{
    if(QPushButton *pb = qobject_cast<QPushButton*>(sender()))
        qDebug() << pb->toolTip();
}

void CLayerWidget::onPbPinClicked()
{
    if(QPushButton *pb = qobject_cast<QPushButton*>(sender()))
        qDebug() << pb->toolTip();
}

void CLayerWidget::onPbUltimateShowClicked()
{
    if(QPushButton *pb = qobject_cast<QPushButton*>(sender()))
        qDebug() << pb->toolTip();
}
