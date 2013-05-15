#include "layerwidget.h"
#include "effectsdlg.h"
#include "layerconstructdlg.h"
#include "IManager.h"

#include <QLayout>
#include <QPushButton>
#include <QFrame>
#include <QDebug>
#include <QResizeEvent>

#include <QToolButton>

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
    _previewWidget = new PreviewWidget(compkey, this);
    layoutMain->addWidget(_previewWidget);

    QVBoxLayout *layoutBtn = new QVBoxLayout(_previewWidget);
    layoutBtn->setSpacing(6);
    layoutBtn->setContentsMargins(0, 0, 0, 0);

    layoutBtn->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    QFrame *frame = new QFrame(_previewWidget);
    frame->setObjectName(QStringLiteral("frame"));
    frame->setStyleSheet(QStringLiteral("#frame {background: rgba(0, 0, 0, 128)}"));
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    QHBoxLayout *horizontalLayout = new QHBoxLayout(frame);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);

    horizontalLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    QSize icon_size = QSize(24,24);

    _pbVisibleHide = new QPushButton(frame);
    _pbVisibleHide->setIconSize(icon_size);
    _pbVisibleHide->setIcon(QIcon(":V_OFF"));
    _pbVisibleHide->setMaximumSize(icon_size);
    _pbVisibleHide->setToolTip(tr("visible/hide"));   
    _pbVisibleHide->setCheckable(true);
    horizontalLayout->addWidget(_pbVisibleHide);
    connect(_pbVisibleHide, SIGNAL(toggled(bool)), SLOT(onPbVisibleHideToggled(bool)));


    QPushButton *pbPin = new QPushButton(frame);
    pbPin->setIconSize(icon_size);
    pbPin->setIcon(QIcon(":P"));
    pbPin->setMaximumSize(icon_size);
    pbPin->setToolTip(tr("pin"));
    pbPin->setCheckable(true);
    horizontalLayout->addWidget(pbPin);
    connect(pbPin, SIGNAL(toggled(bool)), SLOT(onPbPinToggled(bool)));



    //if(type != CLayerWidget::ELayerTypeSUBSCENE)
    {
        QPushButton *pbResize = new QPushButton(frame);
        pbResize->setIconSize(icon_size);
        pbResize->setIcon(QIcon(":R"));
        pbResize->setMaximumSize(icon_size);

        pbResize->setToolTip(tr("resize & reposition"));
        horizontalLayout->addWidget(pbResize);
        connect(pbResize, SIGNAL(clicked()), SLOT(onPbResizeClicked()));
    }

    if(type != CLayerWidget::ELayerTypeSUBSCENE) // для сцен кнопка эффектов НЕ отображается
    {

        QPushButton *pbEffect = new QPushButton(frame);
        pbEffect->setIconSize(icon_size);
        pbEffect->setIcon(QIcon(":E"));
        pbEffect->setMaximumSize(icon_size);
        pbEffect->setToolTip(tr("effect"));
        horizontalLayout->addWidget(pbEffect);
        connect(pbEffect, SIGNAL(clicked()), SLOT(onPbEffectClicked()));
    }

    if(type == CLayerWidget::ELayerTypeSUBSCENE)
    {
        QPushButton *pbConstruct = new QPushButton(frame);
        pbConstruct->setIconSize(icon_size);
        pbConstruct->setIcon(QIcon(":C"));
        pbConstruct->setMaximumSize(icon_size);

        pbConstruct->setToolTip(tr("scene settings"));
        horizontalLayout->addWidget(pbConstruct);
        connect(pbConstruct, SIGNAL(clicked()), SLOT(onPbConstructClicked()));
    }

    QPushButton *pbUltimateShow = new QPushButton(frame);
    pbUltimateShow->setIconSize(icon_size);
    pbUltimateShow->setIcon(QIcon(":U"));
    pbUltimateShow->setMaximumSize(icon_size);
    pbUltimateShow->setToolTip(tr("ultimate show"));
    horizontalLayout->addWidget(pbUltimateShow);
    connect(pbUltimateShow, SIGNAL(clicked()), SLOT(onPbUltimateShowClicked()));


    horizontalLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    layoutBtn->addWidget(frame);

    layoutBtn->setStretch(0, 5);
    layoutBtn->setStretch(1, 1);
}





/*
void CLayerWidget::setEnabledOpenGl(bool enable)
{
#ifndef QT_NO_OPENGL
    if(QGLFormat::hasOpenGL())
        setViewport(enable?new QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DirectRendering)):new QWidget);
#endif
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
    QPoint point;
    point.setX((event->size().width() - btnWidget->width())/2);
    point.setY(event->size().height() - btnWidget->height());
    btnWidget->move(point);
}
*/
void CLayerWidget::onPbVisibleHideToggled(bool checked)
{
    if(QPushButton *pb = qobject_cast<QPushButton*>(sender()))
        qDebug() << pb->toolTip() << " state: " << (pb->isChecked()?"checked":"unchecked");

    if(checked) {
        global_manager->showLayer(_compkey);
    } else {
        global_manager->hideLayer(_compkey);
    }

    setVisibleHide(checked);
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
        CEffectsDlg dlg;
        if(dlg.exec() == QDialog::Accepted){
            QString efname = dlg.getEffect();
            if(efname == "CLEAN"){
                global_manager->removeEffects(this->compKey());
            }else{
                global_manager->applyEffects(this->compKey(), efname.toLocal8Bit().data());
            }
        }
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

    // TODO: здесь нужно узнавать ключ проксируемой сцены
    if(_layerConstructDlg == 0){
        int scene_id = global_manager->getProxySceneId(_compkey);
        _layerConstructDlg = new CLayerConstructDlg(scene_id);
    }
    _layerConstructDlg->show();
}

qint32 CLayerWidget::compKey() const
{
    return _compkey;
}

void CLayerWidget::setVisibleHide(bool visibleHide)
{
    _pbVisibleHide->setChecked(visibleHide);
    if(visibleHide){
        _pbVisibleHide->setIcon(QIcon(":V_ON"));
    }else{
        _pbVisibleHide->setIcon(QIcon(":V_OFF"));
    }

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
