#include "scenepanel.h"
#include "screencapture.h"

#include <QLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QDebug>
#include <QTimer>
#include <QDir>
#include <QShowEvent>
#include <QScrollArea>
#include <QFileDialog>

#include "IManager.h"
#include "settingsmanager.h"
#include "CaptureSelectDialog.h"
#include "htmlrender.h"
#include "imagerender.h"
#include "utils.h"

CScenePanel::CScenePanel(qint32 compkey, QWidget *parent) :
    QWidget(parent),
    _compkey(compkey),
    m_external_count(0),
    _sceneWidget(0)
{    
    _sceneWidget = new CSceneWidget(_compkey, false, this);

    setObjectName("ScenePanel");
    setAttribute(Qt::WA_StyledBackground, true);
}


void CScenePanel::addCamLayer(const QString &sourceName)
{
    addLayer("CAM", sourceName);
}

void CScenePanel::addImageLayer(QString fname)
{
    // получаем размер картинки для формирования имени
    QSize sz = QImage(fname).size();
    //
    m_external_count ++;
    QString name = QString("EXTERNAL_%1_%2_%3x%4").arg(_sceneWidget->getCompkey()).arg(m_external_count).arg(sz.width()).arg(sz.height());

    ImageRender* render = new ImageRender(name, this);
    CLayerWidget* lw = addLayer("EXTERNAL", name, CLayerWidget::ELayerTypeIMAGE);

    //
    connect(render, SIGNAL(newFile(QString)), lw, SLOT(setTitle(QString)));

    // делаем привязку ловли сигналов(next\prev) от lw к render
    connect(lw, SIGNAL(switchImage(bool)), render, SLOT(switchImage(bool)));
    connect(lw, SIGNAL(deleteLayer()), render, SLOT(onDeleteLayer()));

    render->setFile(fname);
}


void CScenePanel::addSubSceneLayer()
{
    CLayerWidget *lw;
    lw = addLayer("SUBSCENE", "", CLayerWidget::ELayerTypeSUBSCENE);
}

void CScenePanel::addScreenCaptureLayer(RectSelectionWidget * w)
{
    qDebug() << "CScenePanel::addScreenCaptureLayer: rect: " << w->geometry();
    m_external_count ++;
    QString name = QString("EXTERNAL_%1_%2_%3x%4").arg(_sceneWidget->getCompkey()).arg(m_external_count).arg(w->width()).arg(w->height());

    ScreenCapture* src = new ScreenCapture(name, w, this);
    CLayerWidget* lw = addLayer("EXTERNAL", name);
    lw->setTitle(tr("Screen capture"));

    connect(lw, SIGNAL(deleteLayer()), src, SLOT(onDeleteLayer()));
}

void CScenePanel::onAddScreenCapture()
{
    RectSelectionWidget * w = new RectSelectionWidget();
    connect(w, SIGNAL(cancelled()),
            w, SLOT(deleteLater()));
    connect(w, SIGNAL(submitted()),
            this, SLOT(onScreenCaptureSelected()));
    w->show();
}

void CScenePanel::onScreenCaptureSelected()
{
    RectSelectionWidget * w = qobject_cast<RectSelectionWidget*>(sender());
    if (!w) {
        qCritical() << "MainWindow::onScreenCaptureSelected(): sender should be RectSelectionWidget";
        return;
    }

    this->addScreenCaptureLayer(w);
}

void CScenePanel::onDeleteLayer()
{
    CLayerWidget *lw = qobject_cast<CLayerWidget*>(sender());
    lw->hide();

    global_manager->deleteLayer(lw->compKey());
    _listLayerWidgets.removeOne(lw);
    rePosition();
}



CLayerWidget *CScenePanel::findLayerWidgetByCompkey(qint32 compkey)
{
    QListIterator<CLayerWidget*> it(_listLayerWidgets);

    while (it.hasNext())
    {
        CLayerWidget *lw = it.next();

        if(lw->compKey() == compkey)
        {
            return lw;
        }

    }

    return NULL;
}

CLayerWidget* CScenePanel::addLayer(const QString &type, const QString &sourceName,CLayerWidget::LayerType lType)
{    
    int layer_compkey;   

    if( lType == CLayerWidget::ELayerTypeSUBSCENE){
        layer_compkey = global_manager->addScene();        
    }else{
        layer_compkey = global_manager->addLayer(_sceneWidget->getCompkey(), type.toLocal8Bit().data(), sourceName.toLocal8Bit().data());
    }

    CLayerWidget *lw = new CLayerWidget(layer_compkey, lType, this);
    connect(lw, SIGNAL(editLayer(qint32)), SLOT(onEditLayer(qint32)));
    connect(lw, SIGNAL(ultimateShow()), SLOT(onUltimateShow()));
    connect(lw, SIGNAL(deleteLayer()), SLOT(onDeleteLayer()));

    _listLayerWidgets.append(lw);
    lw->setTitle( friendlyDeviceName(sourceName) );
    rePosition();

    //
    lw->setZOrder(1500 + 10*_listLayerWidgets.size());

    return lw;
}

void CScenePanel::onEditLayer(qint32 compkey)
{
    _sceneWidget->toggleBox(compkey);
}

void CScenePanel::onUltimateShow()
{
    CLayerWidget *curLW = qobject_cast<CLayerWidget*>(sender());
    QListIterator<CLayerWidget*> it(_listLayerWidgets);

    // для overlay|background слоя ничего не делаем
    if(curLW->mode() != CLayerWidget::NormalMode ){
        return;
    }

    while (it.hasNext())
    {
        CLayerWidget *lw = it.next();

        if(curLW->compKey() == lw->compKey())
        {
            //lw->setVisibleHide(true);
            continue;
        }

        if( (lw->mode() != CLayerWidget::NormalMode) || !lw->isVisibleHide())
            continue;

        lw->setVisibleState(false);
    }

}

void CScenePanel::onImageSelect()
{
    SettingsManager settings("MainWindow");
    QString file = QFileDialog::getOpenFileName(this, tr("Add Image"), settings.getStringValue("default_dir"), "Image Files (*.png *.jpg *.bmp)");
    if (!file.isEmpty())
    {
        QDir curDir(file);
        settings.setValue("default_dir", curDir.absolutePath());
        this->addImageLayer(file);
    }
}

void CScenePanel::onVideoCaptureSelect()
{
    CaptureSelectDialog dlg(CaptureSelectDialog::VideoDevice);
    if(dlg.exec() == QDialog::Accepted){
        this->addCamLayer(dlg.getDevice());
    }

}

void CScenePanel::onAddHtmlRender()
{
    SettingsManager main_settings("MainWindow");
    QString fn = QFileDialog::getOpenFileName(this, tr("Add HTML Plugin"), main_settings.getStringValue("plugin_dir"), "HTML Files (*.html)");

    if (fn.isEmpty())
        return;

    QDir curDir(fn);
    main_settings.setValue("plugin_dir", curDir.absolutePath());



    // получаем размеры рабочей зоны
    SettingsManager setting("Settings");
    QString wsz = setting.getStringValue("Worksize");

    //addHtmlRenderLayer(QUrl::fromLocalFile(fn).toString());
    m_external_count ++;
    QString name = QString("EXTERNAL_%1_%2_%3").arg(_sceneWidget->getCompkey()).arg(m_external_count).arg(wsz);

    HtmlRender* rd = new HtmlRender(name, fn, this);
    CLayerWidget* lw = addLayer("EXTERNAL", name, CLayerWidget::ELayerTypeHTMLPLUGIN);
    lw->setTitle(tr("HTML Plugin"));

    // по умолчанию плагины идут в overlay-mode
    lw->onSetOvrMode();

    // делаем привязку ловли сигналов(open settings) от lw к render
    connect(lw, SIGNAL(openHTMLPluginSettings()), rd, SLOT(onHTMLPluginSettings()));
    connect(lw, SIGNAL(showSignal()), rd, SLOT(onShowSignal()));
    connect(lw, SIGNAL(deleteLayer()), rd, SLOT(onDeleteLayer()));
}

void CScenePanel::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    rePosition();
}


void CScenePanel::rePosition()
{
    int cols, rows;
    int lwsize = _listLayerWidgets.size();

    // определяем кол-во столбцов и строк
    // 0,1 - один столбец
    // 2,3,4 - два столбца
    // > 5   - три столбца
    cols = 3; // default
    if(lwsize < 5){
        cols = lwsize < 2 ? 1:2;
    }

    rows =  lwsize / cols;
    rows = rows == 0? 1: rows;
    if(cols*rows < lwsize)
        rows += 1;

    // делим родительскую ширину между сценой и сеткой слоёв пополам.
    // и высоту сетки делим между слоями.
    int w,h, sw, sh, sx, sy, zw, zh;
    w = this->width();
    h = this->height();


    // параметры сетки
    zh = 7; // высота зазора между строками (два зазора также делается от гор.границ всей области)
    zw = 7; // ширина зазора между столбцами (два зазора также делается слева и справа от всей области)
    w = w - 2*zw;
    h = h - 2*zh;
    sw = (w/2 - (cols)*zw)/cols; // ширина ячейки с учётом зазоров
    sh = (h - (rows-1)*zh)/rows; //  высота ячейки с учётом зазоров

    sx = zw + w/2;
    sy = zh;

    if(_sceneWidget != 0)
    {
        _sceneWidget->setGeometry(zw, zh, w/2, h);
    }

    for(int i=0; i<_listLayerWidgets.size(); i++){
        if( i>0 && i % cols == 0 ){
            sy += sh + zh;
            sx = zw + w/2;
        }

        sx += zw; // вставка зазора между столбцами

        _listLayerWidgets[i]->setGeometry(sx, sy, sw, sh);
        _listLayerWidgets[i]->show();
        sx += sw;
    }

}

void CScenePanel::start()
{
    if(_sceneWidget != 0)
    {
        _sceneWidget->start();
        _sceneWidget->startBox();
    }

    QListIterator<CLayerWidget*> it(_listLayerWidgets);

    while(it.hasNext())
    {
        it.next()->start();
    }
}

void CScenePanel::stop()
{
    if(_sceneWidget != 0)
    {
        _sceneWidget->stop();
        _sceneWidget->stopBox();
    }

    QListIterator<CLayerWidget*> it(_listLayerWidgets);

    while(it.hasNext())
    {
        it.next()->stop();
    }
}

void CScenePanel::applySetting()
{
    SettingsManager setting("Video");
    bool isEnabledOpenGL = setting.getBoolValue("OpenGL");
    //_sceneWidget->setEnabledOpenGl(isEnabledOpenGL);

    QListIterator<CLayerWidget*> it(_listLayerWidgets);
    while(it.hasNext())
    {
        //it.next()->setEnabledOpenGl(isEnabledOpenGL);
    }
}

/*
void CScenePanel::hideLayers()
{
    // скрывает все Normal-слои  ( overlay|background слои не затрагиваются)
    QListIterator<CLayerWidget*> it(_listLayerWidgets);

    while (it.hasNext())
    {
        CLayerWidget *lw = it.next();

        if(  !lw->mode()== CLayerWidget::NormalMode || !lw->isVisibleHide())
            continue;

        lw->setVisibleState(false);
    }
}
*/


