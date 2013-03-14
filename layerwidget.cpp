#include "layerwidget.h"
#include "previewwidget.h"
#include "effectsdlg.h"

#include <QLayout>
#include <QPushbutton>
#include <QDebug>

CLayerWidget::CLayerWidget(int compkey, QWidget *parent) :
    _compkey(compkey),
    QWidget(parent)
{
    QVBoxLayout *verticallLayout = new QVBoxLayout(this);
    verticallLayout->setSpacing(6);
    verticallLayout->setContentsMargins(11, 11, 11, 11);
    verticallLayout->setContentsMargins(0, 0, 0, 0);
    PreviewWidget *pw = new PreviewWidget(compkey, this);
    verticallLayout->addWidget(pw);

    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSpacing(6);
    QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer);

    QPushButton *pbVisibleHide = new QPushButton("V", this);
    pbVisibleHide->setMaximumSize(QSize(20, 16777215));
    pbVisibleHide->setToolTip("visible/hide");
    pbVisibleHide->setCheckable(true);
    horizontalLayout->addWidget(pbVisibleHide);
    connect(pbVisibleHide, SIGNAL(clicked()), SLOT(onPbVisibleHideClicked()));

    QPushButton *pbResize = new QPushButton("R", this);
    pbResize->setMaximumSize(QSize(20, 16777215));
    pbResize->setToolTip("resize");
    horizontalLayout->addWidget(pbResize);
    connect(pbResize, SIGNAL(clicked()), SLOT(onPbResizeClicked()));

    QPushButton *pbEffect = new QPushButton("E", this);
    pbEffect->setMaximumSize(QSize(20, 16777215));
    pbEffect->setToolTip("effect");
    horizontalLayout->addWidget(pbEffect);
    connect(pbEffect, SIGNAL(clicked()), SLOT(onPbEffectClicked()));

    QPushButton *pbPin = new QPushButton("P", this);
    pbPin->setMaximumSize(QSize(20, 16777215));
    pbPin->setToolTip("pin");
    pbPin->setCheckable(true);
    horizontalLayout->addWidget(pbPin);
    connect(pbPin, SIGNAL(clicked()), SLOT(onPbPinClicked()));

    QPushButton *pbUltimateShow = new QPushButton("U", this);
    pbUltimateShow->setMaximumSize(QSize(20, 16777215));
    pbUltimateShow->setToolTip("ultimate show");
    horizontalLayout->addWidget(pbUltimateShow);
    connect(pbUltimateShow, SIGNAL(clicked()), SLOT(onPbUltimateShowClicked()));
        
    
    //pw->setGeometry(0,0,40,40);
    //pw->start(compkey, 40); // TODO: we must start widget on create?
    
    QSpacerItem *horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer_2);
    verticallLayout->addLayout(horizontalLayout);


    verticallLayout->setStretch(0, 2);
}

void CLayerWidget::onPbVisibleHideClicked()
{
    if(QPushButton *pb = qobject_cast<QPushButton*>(sender()))
    {
        qDebug() << pb->toolTip() << " state: " << (pb->isChecked()?"checked":"unchecked");
    }
}

void CLayerWidget::onPbResizeClicked()
{
    if(QPushButton *pb = qobject_cast<QPushButton*>(sender()))
    {
        qDebug() << pb->toolTip();
        emit editLayer(_compkey);
    }
}

void CLayerWidget::onPbEffectClicked()
{
    if(QPushButton *pb = qobject_cast<QPushButton*>(sender()))
    {
        qDebug() << pb->toolTip();
        CEffectsDlg eff;
        eff.setColuntCount(3);
        eff.exec();
        QString effect = eff.selectedEffectName();
        qDebug() << "Effect selected: " << effect;
    }
}

void CLayerWidget::onPbPinClicked()
{
    if(QPushButton *pb = qobject_cast<QPushButton*>(sender()))
        qDebug() << pb->toolTip() << " state: " << (pb->isChecked()?"checked":"unchecked");
}

void CLayerWidget::onPbUltimateShowClicked()
{
    if(QPushButton *pb = qobject_cast<QPushButton*>(sender()))
        qDebug() << pb->toolTip();
}
