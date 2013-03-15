#include "layerwidget.h"
#include "previewwidget.h"
#include "effectsdlg.h"
#include "layerconstructdlg.h"

#include "IManager.h"

#include <QLayout>
#include <QPushbutton>
#include <QFrame>
#include <QDebug>

CLayerWidget::CLayerWidget(int compkey, CLayerWidget::LayerType type, QWidget *parent) :
    _compkey(compkey),
    _pin(false),
    _layerConstructDlg(0),
    QWidget(parent)
{
    QVBoxLayout *layoutMain = new QVBoxLayout(this);
    layoutMain->setSpacing(6);
    layoutMain->setContentsMargins(11, 11, 11, 11);
    layoutMain->setContentsMargins(0, 0, 0, 0);
    PreviewWidget *pw = new PreviewWidget(compkey, this);
    layoutMain->addWidget(pw);

    QVBoxLayout *layoutBtn = new QVBoxLayout(pw);
    layoutBtn->setSpacing(6);
    layoutBtn->setContentsMargins(0, 0, 0, 0);

    layoutBtn->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    QFrame *frame = new QFrame(pw);
    frame->setObjectName(QStringLiteral("frame"));
    frame->setStyleSheet(QStringLiteral("#frame {background: rgba(0, 0, 0, 128)}"));
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    QHBoxLayout *horizontalLayout = new QHBoxLayout(frame);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);

    horizontalLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    _pbVisibleHide = new QPushButton("V", frame);
    _pbVisibleHide->setMaximumSize(QSize(20, 16777215));
    _pbVisibleHide->setToolTip(tr("visible/hide"));
    _pbVisibleHide->setCheckable(true);
    horizontalLayout->addWidget(_pbVisibleHide);
    connect(_pbVisibleHide, SIGNAL(toggled(bool)), SLOT(onPbVisibleHideToggled(bool)));

    QPushButton *pbResize = new QPushButton("R", frame);
    pbResize->setMaximumSize(QSize(20, 16777215));
    pbResize->setToolTip(tr("resize"));
    horizontalLayout->addWidget(pbResize);
    connect(pbResize, SIGNAL(clicked()), SLOT(onPbResizeClicked()));

    QPushButton *pbEffect = new QPushButton("E", frame);
    pbEffect->setMaximumSize(QSize(20, 16777215));
    pbEffect->setToolTip(tr("effect"));
    horizontalLayout->addWidget(pbEffect);
    connect(pbEffect, SIGNAL(clicked()), SLOT(onPbEffectClicked()));

    QPushButton *pbPin = new QPushButton("P", frame);
    pbPin->setMaximumSize(QSize(20, 16777215));
    pbPin->setToolTip(tr("pin"));
    pbPin->setCheckable(true);
    horizontalLayout->addWidget(pbPin);
    connect(pbPin, SIGNAL(toggled(bool)), SLOT(onPbPinToggled(bool)));

    QPushButton *pbUltimateShow = new QPushButton("U", frame);
    pbUltimateShow->setMaximumSize(QSize(20, 16777215));
    pbUltimateShow->setToolTip(tr("ultimate show"));
    horizontalLayout->addWidget(pbUltimateShow);
    connect(pbUltimateShow, SIGNAL(clicked()), SLOT(onPbUltimateShowClicked()));

    if(type == LayerType::ELayerTypeSUBSCENE)
    {
        QPushButton *pbConstruct = new QPushButton("C", frame);
        pbConstruct->setMaximumSize(QSize(20, 16777215));
        pbConstruct->setToolTip(tr("construct"));
        horizontalLayout->addWidget(pbConstruct);
        connect(pbConstruct, SIGNAL(clicked()), SLOT(onPbConstructClicked()));
    }
    
    horizontalLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    layoutBtn->addWidget(frame);

    layoutBtn->setStretch(0, 5);
    layoutBtn->setStretch(1, 1);
}

CLayerWidget::~CLayerWidget()
{
    if(_pbVisibleHide != 0)
    {
        delete _pbVisibleHide;
        _pbVisibleHide = 0;
    }
}

void CLayerWidget::onPbVisibleHideToggled(bool checked)
{
    if(QPushButton *pb = qobject_cast<QPushButton*>(sender()))
        qDebug() << pb->toolTip() << " state: " << (pb->isChecked()?"checked":"unchecked");

    if(checked) {
        global_manager->showLayer(_compkey);
    } else {
        global_manager->hideLayer(_compkey);
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

void CLayerWidget::onPbPinToggled(bool checked)
{
    if(QPushButton *pb = qobject_cast<QPushButton*>(sender()))
        qDebug() << pb->toolTip() << " state: " << (pb->isChecked()?"checked":"unchecked");

    _pin = checked;
}

void CLayerWidget::onPbUltimateShowClicked()
{
    if(QPushButton *pb = qobject_cast<QPushButton*>(sender()))
    {
        qDebug() << pb->toolTip();
        emit ultimateShow();
    }
}

void CLayerWidget::onPbConstructClicked()
{
    if(QPushButton *pb = qobject_cast<QPushButton*>(sender()))
        qDebug() << pb->toolTip();

    if(_layerConstructDlg == 0)
        _layerConstructDlg = new CLayerConstructDlg();
    _layerConstructDlg->setWindowTitle(QString("Sub Scene N %1").arg(_compkey));
    _layerConstructDlg->show();
}

qint32 CLayerWidget::compKey() const
{
    return _compkey;
}

void CLayerWidget::setVisibleHide(bool visibleHide)
{
    _pbVisibleHide->setChecked(visibleHide);
}

bool CLayerWidget::isVisibleHide() const
{
    return _pbVisibleHide->isChecked();
}

bool CLayerWidget::isPinEnable() const
{
    return _pin;
}
