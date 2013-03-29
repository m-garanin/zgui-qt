#include "layerwidget.h"
#include "graphicsitem.h"
#include "effectsdlg.h"
#include "layerconstructdlg.h"
#include "IManager.h"

#include <QLayout>
#include <QPushButton>
#include <QFrame>
#include <QDebug>
#include <QResizeEvent>

#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#endif

CLayerWidget::CLayerWidget(qint32 compkey, CLayerWidget::LayerType type, QWidget *parent) :
    QGraphicsView(parent),
    _compkey(compkey),
    _pin(false),
    _layerType(type),
    _layerConstructDlg(0)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);
    setTransformationAnchor(AnchorUnderMouse);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing
                                      | QGraphicsView::DontClipPainter
                                      | QGraphicsView::DontSavePainterState);
    setMouseTracking(true);
    CGraphicsItem *background = new CGraphicsItem(_compkey);
    background->setPos(0,0);
    background->setImageFitMode(CGraphicsItem::ImageFit);
    scene->addItem(background);

    initBtn();

#ifndef QT_NO_OPENGL
    //setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DirectRendering), this));
#endif

    _timerId = startTimer(1000 / 25);
}

void CLayerWidget::initBtn()
{
    QVBoxLayout *layoutMain = new QVBoxLayout();
    layoutMain->setSpacing(6);
    layoutMain->setContentsMargins(11, 11, 11, 11);
    layoutMain->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *layoutBtn = new QVBoxLayout(this);
    layoutBtn->setSpacing(6);
    layoutBtn->setContentsMargins(0, 0, 0, 0);

    layoutBtn->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);

    horizontalLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    _pbVisibleHide = new QPushButton("V", this);
    _pbVisibleHide->setMaximumSize(QSize(20, 16777215));
    _pbVisibleHide->setToolTip(tr("visible/hide"));
    _pbVisibleHide->setCheckable(true);
    horizontalLayout->addWidget(_pbVisibleHide);
    connect(_pbVisibleHide, SIGNAL(toggled(bool)), SLOT(onPbVisibleHideToggled(bool)));

    QPushButton *pbResize = new QPushButton("R", this);
    pbResize->setMaximumSize(QSize(20, 16777215));
    pbResize->setToolTip(tr("resize"));
    horizontalLayout->addWidget(pbResize);
    connect(pbResize, SIGNAL(clicked()), SLOT(onPbResizeClicked()));

    QPushButton *pbEffect = new QPushButton("E", this);
    pbEffect->setMaximumSize(QSize(20, 16777215));
    pbEffect->setToolTip(tr("effect"));
    horizontalLayout->addWidget(pbEffect);
    connect(pbEffect, SIGNAL(clicked()), SLOT(onPbEffectClicked()));

    QPushButton *pbPin = new QPushButton("P", this);
    pbPin->setMaximumSize(QSize(20, 16777215));
    pbPin->setToolTip(tr("pin"));
    pbPin->setCheckable(true);
    horizontalLayout->addWidget(pbPin);
    connect(pbPin, SIGNAL(toggled(bool)), SLOT(onPbPinToggled(bool)));

    QPushButton *pbUltimateShow = new QPushButton("U", this);
    pbUltimateShow->setMaximumSize(QSize(20, 16777215));
    pbUltimateShow->setToolTip(tr("ultimate show"));
    horizontalLayout->addWidget(pbUltimateShow);
    connect(pbUltimateShow, SIGNAL(clicked()), SLOT(onPbUltimateShowClicked()));

    if(_layerType == CLayerWidget::ELayerTypeSUBSCENE)
    {
        QPushButton *pbConstruct = new QPushButton("C", this);
        pbConstruct->setMaximumSize(QSize(20, 16777215));
        pbConstruct->setToolTip(tr("construct"));
        horizontalLayout->addWidget(pbConstruct);
        connect(pbConstruct, SIGNAL(clicked()), SLOT(onPbConstructClicked()));
    }

    horizontalLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    layoutBtn->addLayout(horizontalLayout);
    layoutMain->addWidget(this);
}

void CLayerWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    QListIterator<QGraphicsItem*> it(scene()->items());
    while(it.hasNext())
    {
        it.next()->update();
    }
}

void CLayerWidget::resizeEvent(QResizeEvent *event)
{
    QListIterator<QGraphicsItem*> it(scene()->items());
    while(it.hasNext())
    {
        CGraphicsItem *item = qgraphicsitem_cast<CGraphicsItem*>(it.next());
        item->setSize(event->size());
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
        _layerConstructDlg = new CLayerConstructDlg(_compkey);
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

void CLayerWidget::start()
{
    // only one timer
    if(_timerId == 0)
        startTimer(1000 / 25);
}

void CLayerWidget::stop()
{
    killTimer(_timerId);
    qDebug() << "_timerId " << _timerId;
}
