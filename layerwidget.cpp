#include "layerwidget.h"
#include "effectsdlg.h"
#include "layerconstructdlg.h"


#include <QLayout>
#include <QPushButton>
#include <QFrame>
#include <QDebug>
#include <QToolButton>
#include <QMessageBox>
#include <QSlider>

CLayerWidget::CLayerWidget(int compkey, CLayerWidget::LayerType type, QWidget *parent) :
    _compkey(compkey),
    _pin(false),
    _is_visible(false),
    _layerConstructDlg(0),
    _layerType(type),
    m_mode(LayerMode::NormalMode),    
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

    layoutBtn->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));

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

        // folder-button
        pbConstruct = new QPushButton(frame);
        pbConstruct->setIconSize(icon_size);
        pbConstruct->setIcon(QIcon(":FOLDER"));
        pbConstruct->setMaximumSize(icon_size);

        pbConstruct->setToolTip(tr("select image"));
        horizontalLayout->addWidget(pbConstruct);
        connect(pbConstruct, SIGNAL(clicked()), SLOT(onSelectImage()));


        // next-button
        pbConstruct = new QPushButton(frame);
        pbConstruct->setIconSize(icon_size);
        pbConstruct->setIcon(QIcon(":I_NEXT"));
        pbConstruct->setMaximumSize(icon_size);

        pbConstruct->setToolTip(tr("next image"));
        horizontalLayout->addWidget(pbConstruct);
        connect(pbConstruct, SIGNAL(clicked()), SLOT(onNextImage()));
    }

    if(type == CLayerWidget::ELayerTypeVIDEO){
        //
        //

        m_playback_slider = new QSlider(Qt::Horizontal, _previewWidget);
        layoutBtn->addWidget(m_playback_slider);

        // Play-button
        m_playback_play = true;
        m_pbPlay = new QPushButton(frame);
        m_pbPlay->setIconSize(icon_size);
        m_pbPlay->setIcon(QIcon(":PLAY"));
        m_pbPlay->setMaximumSize(icon_size);

        m_pbPlay->setToolTip(tr("Play"));
        horizontalLayout->addWidget(m_pbPlay);
        connect(m_pbPlay, SIGNAL(clicked()), SLOT(onPlayClicked()));

        global_manager->queryIPlaybackSource(compkey, &m_playback);

        m_playback_slider->setTickInterval(5);
        m_playback_slider->setMaximum(m_playback->getDuration());

        connect(m_playback_slider, SIGNAL(actionTriggered(int)), SLOT(onPlaybackSliderClick()));


        m_playback_timer = new QTimer(this);
        connect(m_playback_timer, SIGNAL(timeout()), this, SLOT(onPlaybackTimer()));
        m_playback_timer->start(500);
    }


    m_pbMode = new QPushButton(frame);
    m_pbMode->setIconSize(icon_size);
    m_pbMode->setMaximumSize(icon_size);
    m_pbMode->hide();
    horizontalLayout->addWidget(m_pbMode);
    horizontalLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));



    layoutBtn->addWidget(frame);

    //layoutBtn->setStretch(0, 2);
    //layoutBtn->setStretch(1, 1);






    //////////////////////////////////////////////////////////////////////////////////
    //                                  CONTEXT-MENU
    //////////////////////////////////////////////////////////////////////////////////

    setContextMenuPolicy(Qt::DefaultContextMenu);
    QAction * act;
    m_contextMenu = new QMenu(this);

    // overlay|normal|background mode

    act = new QAction(tr("Mode: overlay"), this);
    connect(act, SIGNAL(triggered()), this, SLOT(onSetOvrMode()));
    addAction(act);

    act = new QAction(tr("Mode: normal"), this);
    connect(act, SIGNAL(triggered()), this, SLOT(setMode()));
    addAction(act);

    act = new QAction(tr("Mode: background"), this);
    connect(act, SIGNAL(triggered()), this, SLOT(onSetBkgMode()));
    addAction(act);


    act = new QAction(tr("Size: optimal"), this);
    connect(act, SIGNAL(triggered()), this, SLOT(onSetOptimalPos()));
    addAction(act);

    act = new QAction(tr("Size: fill"), this);
    connect(act, SIGNAL(triggered()), this, SLOT(onSetFullPos()));
    addAction(act);

    act = new QAction(tr("Move: up"), this);
    connect(act, SIGNAL(triggered()), this, SLOT(onMoveUp()));
    addAction(act);

    act = new QAction(tr("Move: down"), this);
    connect(act, SIGNAL(triggered()), this, SLOT(onMoveDown()));
    addAction(act);


    act = new QAction(tr("Delete hidden layer"), this);
    connect(act, SIGNAL(triggered()), this, SLOT(onDelete()));
    addAction(act);
    m_delete_action = act;


    m_contextMenu->addActions(this->actions());


    if(type == CLayerWidget::ELayerTypeSUBSCENE){
        int scene_id = global_manager->getProxySceneId(_compkey);
        _layerConstructDlg = new CLayerConstructDlg(scene_id);
    }

}


void CLayerWidget::onPbVisibleClicked()
{
    if( !_is_visible ){
        emit ultimateShow();
    }

    setVisibleState(!_is_visible);
}

/*
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
*/
void CLayerWidget::onNextImage()
{
    emit switchImage(true);
}

void CLayerWidget::onPrevImage()
{
    emit switchImage(false);
}

void CLayerWidget::onSelectImage()
{
    emit selectImage();
}

void CLayerWidget::onHTMLPluginSettings()
{
    emit openHTMLPluginSettings();
}

void CLayerWidget::onSetOptimalPos()
{
    global_manager->setLayerOptimalSize(compKey());
}

void CLayerWidget::onSetFullPos()
{
    global_manager->setLayerFullSize(compKey());
}

void CLayerWidget::onSetBkgMode()
{
    setMode(LayerMode::BkgMode);
}

void CLayerWidget::onSetOvrMode()
{
    setMode(LayerMode::OvrMode);
}

void CLayerWidget::setMode(CLayerWidget::LayerMode md)
{
    m_mode = md;    
    if(md == LayerMode::BkgMode){
        m_pbMode->show();
        m_pbMode->setIcon(QIcon(":BKG_FLAG"));
        setZOrder(m_zorder % 1000); // опускаем на уровень "до 1000"
        return;
    }

    if(md == LayerMode::OvrMode){
        m_pbMode->show();
        m_pbMode->setIcon(QIcon(":OVR_FLAG"));
        setZOrder(2000 + m_zorder % 1000); // поднимаем на уровень "выше 2000"
        return;
    }

    setZOrder(1000 + m_zorder % 1000); // на уровень [1000,2000)
    m_pbMode->hide();
}

void CLayerWidget::onMoveUp()
{
    setZOrder(m_zorder + 10);
}

void CLayerWidget::onMoveDown()
{
    setZOrder(m_zorder - 15);
}

void CLayerWidget::onDelete()
{
    if(QMessageBox::question(this, tr("Delete layer"), tr("Are you sure?")) == QMessageBox::Yes ){
        //
        _previewWidget->stop();
        if(getLayerType() == ELayerTypeSUBSCENE){
            _layerConstructDlg->deleteLater();
        }
        emit deleteLayer(); // сигнал должны ловить scenepanel и возможно html-render, screen-capture etc...
    }
}

void CLayerWidget::onPlayClicked()
{
    if(m_playback_play){
        // make pause
        m_pbPlay->setIcon(QIcon(":PLAY_WAIT"));
        m_playback->pause();
    }else{
        // start play
        m_pbPlay->setIcon(QIcon(":PLAY"));
        m_playback->play();
    }
    m_playback_play = !m_playback_play;
}

void CLayerWidget::onPlaybackSliderClick()
{
    qDebug() << "onPlaybackSliderReleased" << m_playback_slider->value();
    m_playback->seek(m_playback_slider->value());
}

void CLayerWidget::onPlaybackTimer()
{
    if(m_playback_play){
        m_playback_slider->setValue( m_playback->getPosition() );
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

    /*
    // TODO: здесь нужно узнавать ключ проксируемой сцены
    if(_layerConstructDlg == 0){
        int scene_id = global_manager->getProxySceneId(_compkey);
        _layerConstructDlg = new CLayerConstructDlg(scene_id);
    }
    */
    _layerConstructDlg->show();
}

qint32 CLayerWidget::compKey() const
{
    return _compkey;
}

void CLayerWidget::setVisibleState(bool visible)
{
    _is_visible = visible;

    m_delete_action->setDisabled(visible);

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
   _previewWidget->start();
}

void CLayerWidget::stop()
{
    _previewWidget->stop();    
}

void CLayerWidget::setZOrder(int zorder)
{
    m_zorder = zorder;
    global_manager->setZOrder(compKey(), m_zorder);
}

void CLayerWidget::setTitle(QString txt)
{
    //qDebug() << "SET TITLE" << txt << this->_title;
    _title->setText(txt);

}


void CLayerWidget::contextMenuEvent(QContextMenuEvent *event)
{
    m_contextMenu->exec(event->globalPos());
}


QString CLayerWidget::typeAsString(){
    if(_layerType == ELayerTypeHTMLPLUGIN)
        return "HTMLPLUGIN";

    if(_layerType == ELayerTypeSUBSCENE)
        return "SUBSCENE";

    if(_layerType == ELayerTypeIMAGE)
        return "IMAGE";

    if(_layerType == ELayerTypeCAM)
        return "CAM";

    if(_layerType == ELayerTypeSCREEN)
         return "SCREEN";

    if(_layerType == ELayerTypeNETWORK)
         return "NETWORK";

    return "DEFAULT";
}

QString CLayerWidget::modeAsString(){
    if(m_mode == BkgMode)
        return "BKG";

    if(m_mode == OvrMode)
        return "OVR";

    return "NORMAL";
}

void *CLayerWidget::getProxyScenePanel()
{
    if(_layerConstructDlg==0)
        return NULL;

    return _layerConstructDlg->getScenePanel();
}


