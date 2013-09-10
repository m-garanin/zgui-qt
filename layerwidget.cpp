#include "layerwidget.h"
#include "effectsdlg.h"
#include "layerconstructdlg.h"
#include "IManager.h"

#include <QLayout>
#include <QPushButton>
#include <QFrame>
#include <QDebug>
#include <QToolButton>

CLayerWidget::CLayerWidget(int compkey, CLayerWidget::LayerType type, QWidget *parent) :
    _compkey(compkey),
    _pin(false),
    _is_visible(false),
    _layerConstructDlg(0),
    QWidget(parent)
{    
    QVBoxLayout *layoutMain = new QVBoxLayout(this);
    layoutMain->setSpacing(6);
    layoutMain->setContentsMargins(11, 11, 11, 11);
    layoutMain->setContentsMargins(0, 0, 0, 0);


    _previewWidget = new PreviewWidget(compkey, false, this);
    layoutMain->addWidget(_previewWidget);

    _title = new QLineEdit("", this);
    _title->setObjectName("LayerTitle");
    _title->setAlignment(Qt::AlignHCenter);
    layoutMain->addWidget(_title);

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
    horizontalLayout->addWidget(_pbVisibleHide);
    connect(_pbVisibleHide, SIGNAL(clicked()), SLOT(onPbVisibleClicked()));



    // отключаем позиционирование для некоторых типов
    if(type != CLayerWidget::ELayerTypeHTMLPLUGIN)
    {
        QPushButton *pbResize = new QPushButton(frame);
        pbResize->setIconSize(icon_size);
        pbResize->setIcon(QIcon(":R"));
        pbResize->setMaximumSize(icon_size);

        pbResize->setToolTip(tr("resize & reposition"));
        horizontalLayout->addWidget(pbResize);
        connect(pbResize, SIGNAL(clicked()), SLOT(onPbResizeClicked()));
    }
    /*
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
    */

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

    if(type == CLayerWidget::ELayerTypeHTMLPLUGIN)
    {
        QPushButton *pbConstruct = new QPushButton(frame);
        pbConstruct->setIconSize(icon_size);
        pbConstruct->setIcon(QIcon(":C"));
        pbConstruct->setMaximumSize(icon_size);

        pbConstruct->setToolTip(tr("plugin settings"));
        horizontalLayout->addWidget(pbConstruct);
        connect(pbConstruct, SIGNAL(clicked()), SLOT(onHTMLPluginSettings()));
    }
    /*
    QPushButton *pbUltimateShow = new QPushButton(frame);
    pbUltimateShow->setIconSize(icon_size);
    pbUltimateShow->setIcon(QIcon(":U"));
    pbUltimateShow->setMaximumSize(icon_size);
    pbUltimateShow->setToolTip(tr("ultimate show"));
    horizontalLayout->addWidget(pbUltimateShow);
    connect(pbUltimateShow, SIGNAL(clicked()), SLOT(onPbUltimateShowClicked()));
    */

    if(type == CLayerWidget::ELayerTypeIMAGE)
    {
        // prev-button
        QPushButton *pbConstruct = new QPushButton(frame);
        pbConstruct->setIconSize(icon_size);
        pbConstruct->setIcon(QIcon(":I_PREV"));
        pbConstruct->setMaximumSize(icon_size);

        pbConstruct->setToolTip(tr("previous image"));
        horizontalLayout->addWidget(pbConstruct);
        connect(pbConstruct, SIGNAL(clicked()), SLOT(onPrevImage()));

        // next-button
        pbConstruct = new QPushButton(frame);
        pbConstruct->setIconSize(icon_size);
        pbConstruct->setIcon(QIcon(":I_NEXT"));
        pbConstruct->setMaximumSize(icon_size);

        pbConstruct->setToolTip(tr("next image"));
        horizontalLayout->addWidget(pbConstruct);
        connect(pbConstruct, SIGNAL(clicked()), SLOT(onNextImage()));
    }




    _pbPin = new QPushButton(frame);
    _pbPin->setIconSize(icon_size);
    _pbPin->setIcon(QIcon(":P_OFF"));
    _pbPin->setMaximumSize(icon_size);
    _pbPin->setToolTip(tr("pin(always top)"));
    _pbPin->setCheckable(true);
    horizontalLayout->addWidget(_pbPin);
    connect(_pbPin, SIGNAL(toggled(bool)), SLOT(onPbPinToggled(bool)));




    horizontalLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    layoutBtn->addWidget(frame);

    layoutBtn->setStretch(0, 5);
    layoutBtn->setStretch(1, 1);
}


void CLayerWidget::onPbVisibleClicked()
{
    if( !_is_visible ){
        emit ultimateShow();
    }
    // если идёт скрытие и при этом включён пин, то пин отключаем
    if(_is_visible && _pin){
        //
        _pbPin->setChecked(false);
    }

    setVisibleState(!_is_visible);
}

void CLayerWidget::onPbPinToggled(bool checked)
{
    QPushButton *pb = qobject_cast<QPushButton*>(sender());
    if(checked){
        pb->setIcon(QIcon(":P_ON"));
        global_manager->setZOrder(this->compKey(), 100);
        this->setVisibleState(true);
    }else{
        pb->setIcon(QIcon(":P_OFF"));
        global_manager->setZOrder(this->compKey(), 90);

    }
    _pin = checked;
}

void CLayerWidget::onNextImage()
{
    emit switchImage(true);
}

void CLayerWidget::onPrevImage()
{
    emit switchImage(false);
}

void CLayerWidget::onHTMLPluginSettings()
{
    emit openHTMLPluginSettings();
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

void CLayerWidget::setVisibleState(bool visible)
{
    _is_visible = visible;

    if(visible){
        global_manager->showLayer(_compkey);
        _pbVisibleHide->setIcon(QIcon(":V_ON"));
        emit showSignal();
    }else{
        global_manager->hideLayer(_compkey);
        _pbVisibleHide->setIcon(QIcon(":V_OFF"));
    }

}

bool CLayerWidget::isVisibleHide() const
{
    return _is_visible;
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

void CLayerWidget::setTitle(QString txt)
{
    //qDebug() << "SET TITLE" << txt << this->_title;
    _title->setText(txt);

}
