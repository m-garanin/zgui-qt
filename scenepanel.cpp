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
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


#include "IManager.h"
#include "settingsmanager.h"
#include "CaptureSelectDialog.h"
#include "htmlrender.h"
#include "imagerender.h"
#include "mainwindow.h"
#include "audiopanel.h"
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


CLayerWidget* CScenePanel::addCamLayer(const QString &sourceName, QString psize)
{
    if(psize == "AUTO"){
        psize.clear();
    }

    CLayerWidget* lw = addLayer("CAM", sourceName, CLayerWidget::ELayerTypeCAM, psize);
    lw->setPersistentSourceId(sourceName);
    return lw;
}

CLayerWidget* CScenePanel::addImageLayer(QString fname)
{
    // получаем размер картинки для формирования имени
    QSize sz = QImage(fname).size();
    //
    m_external_count ++;
    QString name = QString("EXTERNAL_%1_%2_%3x%4").arg(_sceneWidget->getCompkey()).arg(m_external_count).arg(sz.width()).arg(sz.height());

    ImageRender* render = new ImageRender(name, this);
    CLayerWidget* lw = addLayer("EXTERNAL", name, CLayerWidget::ELayerTypeIMAGE);

    lw->setPersistentSourceId(fname);

    //
    connect(render, SIGNAL(newFile(QString)), lw, SLOT(setTitle(QString)));

    // делаем привязку ловли сигналов(next\prev) от lw к render
    connect(lw, SIGNAL(switchImage(bool)), render, SLOT(switchImage(bool)));
    connect(lw, SIGNAL(deleteLayer()), render, SLOT(onDeleteLayer()));

    render->setFile(fname);

    return lw;
}

CLayerWidget* CScenePanel::addHtmlPluginLayer(QString fname)
{
    // получаем размеры рабочей зоны
    QString wsz = getWorksize();

    m_external_count ++;
    QString name = QString("EXTERNAL_%1_%2_%3").arg(_sceneWidget->getCompkey()).arg(m_external_count).arg(wsz);

    HtmlRender* rd = new HtmlRender(name, fname, this);
    CLayerWidget* lw = addLayer("EXTERNAL", name, CLayerWidget::ELayerTypeHTMLPLUGIN);
    lw->setTitle(tr("HTML Plugin"));
    lw->setPersistentSourceId(fname);
    // по умолчанию плагины идут в overlay-mode
    lw->onSetOvrMode();

    // делаем привязку ловли сигналов(open settings) от lw к render
    connect(lw, SIGNAL(openHTMLPluginSettings()), rd, SLOT(onHTMLPluginSettings()));
    connect(lw, SIGNAL(showSignal()), rd, SLOT(onShowSignal()));
    connect(lw, SIGNAL(deleteLayer()), rd, SLOT(onDeleteLayer()));

    return lw;
}


CLayerWidget* CScenePanel::addSubSceneLayer()
{
    CLayerWidget *lw;
    lw = addLayer("SUBSCENE", "", CLayerWidget::ELayerTypeSUBSCENE);
    lw->setPersistentSourceId("SUBSCENE_IGNORE");
    return lw;
}

CLayerWidget* CScenePanel::addScreenCaptureLayer(QRect rect)
{
    qDebug() << "CScenePanel::addScreenCaptureLayer: rect: " << rect;
    m_external_count ++;
    QString name = QString("EXTERNAL_%1_%2_%3x%4").arg(_sceneWidget->getCompkey()).arg(m_external_count).arg(rect.width()).arg(rect.height());

    ScreenCapture* src = new ScreenCapture(name, rect, this);
    CLayerWidget* lw = addLayer("EXTERNAL", name, CLayerWidget::ELayerTypeSCREEN);
    lw->setTitle(tr("Screen capture"));

    lw->setPersistentSourceId(QString("%1,%2,%3,%4").arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height()));

    connect(lw, SIGNAL(deleteLayer()), src, SLOT(onDeleteLayer()));

    return lw;
}

CLayerWidget *CScenePanel::addVideoFileLayer(QString fname)
{
    //m_external_count ++;

    //QString name = QString("VIDEOFILE_%1_%2").arg(_sceneWidget->getCompkey()).arg(m_external_count);

    //ImageRender* render = new ImageRender(name, this);
    CLayerWidget* lw = addLayer("VIDEOFILE", fname, CLayerWidget::ELayerTypeVIDEO);

    lw->setPersistentSourceId(fname);

    //
    //connect(render, SIGNAL(newFile(QString)), lw, SLOT(setTitle(QString)));

    // делаем привязку ловли сигналов(next\prev) от lw к render
    //connect(lw, SIGNAL(switchImage(bool)), render, SLOT(switchImage(bool)));
    //connect(lw, SIGNAL(deleteLayer()), render, SLOT(onDeleteLayer()));
    //render->setFile(fname);

    return lw;


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

    this->addScreenCaptureLayer(w->grab_geometry());
    w->close();
    w->deleteLater();
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

CLayerWidget* CScenePanel::addLayer(const QString &type, const QString &sourceName,CLayerWidget::LayerType lType, QString strinfo)
{    
    int layer_compkey;   

    if( lType == CLayerWidget::ELayerTypeSUBSCENE){
        layer_compkey = global_manager->addScene();        
    }else{
        layer_compkey = global_manager->addLayer(_sceneWidget->getCompkey(),
                                                 type.toLocal8Bit().data(),
                                                 sourceName.toLocal8Bit().data(),
                                                 strinfo.isEmpty()?NULL:strinfo.toLocal8Bit().data());
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
    if(curLW->getLayerMode() != CLayerWidget::NormalMode ){
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

        if( (lw->getLayerMode() != CLayerWidget::NormalMode) || !lw->isVisibleHide())
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
        this->addCamLayer(dlg.getDevice(), dlg.getPreferableSize());
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

    addHtmlPluginLayer(fn);
}

void CScenePanel::onVideoFileSelect()
{
    SettingsManager settings("MainWindow");
    QString file = QFileDialog::getOpenFileName(this, tr("Add video file"), settings.getStringValue("videofile_dir"), "Video Files (*.avi *.mp4 *.mov *.mkv)");
    if (!file.isEmpty())
    {
        QDir curDir(file);
        settings.setValue("videofile_dir", curDir.absolutePath());
        this->addVideoFileLayer(file);
    }
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



QJsonObject CScenePanel::saveState()
{
    qDebug() << "SAVE STATE";    
    QJsonArray arr;
    CLayerWidget* lw;
    double x,y,w,h;
    int z;

    QListIterator<CLayerWidget*> it(_listLayerWidgets);
    while(it.hasNext())
    {
        QJsonObject obj;
        lw = it.next();
        global_manager->getLayerPosition(lw->compKey(), &x, &y, &w, &h, &z);
        qDebug() << "POSITION:" << x << y << w << h;

        obj.insert("source_id", lw->getPersistentSourceId());
        obj.insert("title", lw->getTitle() );
        obj.insert("type", lw->typeAsString());
        obj.insert("mode", lw->modeAsString());
        obj.insert("zorder", z);

        obj.insert("x", x);
        obj.insert("y", y);
        obj.insert("width", w);
        obj.insert("height", h);

        obj.insert("visible", lw->isVisibleHide());

        // отдельная ветка для подсцен
        if(lw->getLayerType() == CLayerWidget::ELayerTypeSUBSCENE){
            CScenePanel* panel = (CScenePanel*)lw->getProxyScenePanel();
            obj.insert("subscene_state", panel->saveState() );
        }



        arr.append(obj);
    }

    QJsonObject mobj;
    mobj.insert("worksize", getWorksize());
    mobj.insert("layers", arr);
    // TODO: сохранение эффекта

    if(is_main()){
        // сохраняем аудио-панель
        CAudioPanel* ap = ((MainWindow*)window())->getAudioPanel();
        QJsonObject aud = ap->saveState();        
        mobj.insert("audio", aud);
    }

    return mobj;
}

void CScenePanel::restoreState(QJsonObject mobj)
{    
    QJsonArray arr;

    // TODO: восстановление worksize

    arr = mobj.take("layers").toArray();
    for(int i=0;i<arr.size();i++){
        restoreLayer(arr.at(i).toObject());
    }

    if(is_main()){
        // для главной сцены восстанавливаем аудио-панель
        QJsonObject aud = mobj.value("audio").toObject();
        CAudioPanel* ap = ((MainWindow*)window())->getAudioPanel();
        ap->restoreState(aud);
    }

    rePosition();
}


void CScenePanel::restoreLayer(QJsonObject obj)
{
    CLayerWidget* lw;
    QString typ = obj.value("type").toString();
    QString mode = obj.value("mode").toString();
    QString source_id = obj.value("source_id").toString();
    QString title = obj.value("title").toString();
    double x,y, w,h;

    lw = NULL;

    if(typ == "HTMLPLUGIN"){
        if( QFile::exists(source_id) ){
            lw = addHtmlPluginLayer(source_id);
        }
    }


    if(typ == "SUBSCENE" && this->is_main()){        
        // подсцены могут восстанавливаться только для главной сцены
        lw = addSubSceneLayer();
        // восстановление вложенных слоёв
        CScenePanel* p = (CScenePanel*)lw->getProxyScenePanel();
        p->restoreState(obj.value("subscene_state").toObject());            
    }

    if(typ == "IMAGE"){        
        if( QFile::exists(source_id) ){
            lw = addImageLayer(source_id);
            title = source_id;
        }
    }

    if(typ == "SCREEN"){        
        QStringList p = source_id.split(",");
        QRect rect(p[0].toInt(),p[1].toInt(),p[2].toInt(),p[3].toInt());
        lw = addScreenCaptureLayer(rect);
    }

    if(typ == "CAM"){
        // проверяем что камера есть в источниках
        QStringList devs = getVideoCaptureDevices();
        if( devs.contains(source_id) ){ lw = addCamLayer(source_id, "AUTO");} // TODO: может быть не только "AUTO"

    }

    if(lw == NULL)
        return;


    lw->setTitle(title);

    x = obj.value("x").toDouble();
    y = obj.value("y").toDouble();
    w = obj.value("width").toDouble();
    h = obj.value("height").toDouble();

    if(w*h != 0){
        global_manager->repositionLayer(lw->compKey(), x, y, w, h);
    }

    if(obj.value("visible").toBool())
        lw->setVisibleState(true);

    // mode
    if(mode == "OVR"){
        lw->setMode(CLayerWidget::OvrMode);
    }else if(mode == "BGR"){
        lw->setMode(CLayerWidget::BkgMode);
    }

    // zorder
    lw->setZOrder(obj.value("zorder").toDouble());

}

void CScenePanel::onSaveState()
{
    SettingsManager settings("MainWindow");
    QString fname = QFileDialog::getSaveFileName(this, tr("Save config"), settings.getStringValue("config_dir") , "ZGUI Config Files (*.txt)");

    if (fname.isEmpty())
        return;

    QDir curDir(fname);
    settings.setValue("config_dir", curDir.absolutePath());

    saveStateToFile(fname);
}

void CScenePanel::saveStateToFile(QString fname)
{
    QFile file(fname);
    file.open(QIODevice::WriteOnly);

    QJsonDocument doc(saveState());
    file.write( doc.toJson() );

    file.close();
}


void CScenePanel::onRestoreState()
{
    SettingsManager settings("MainWindow");
    QString fname = QFileDialog::getOpenFileName(this, tr("Restore config"), settings.getStringValue("config_dir") , "ZGUI Config Files (*.txt)");

    if (fname.isEmpty())
        return;

    QDir curDir(fname);
    settings.setValue("config_dir", curDir.absolutePath());

    restoreStateFromFile(fname);
}

void CScenePanel::onAddPlayback()
{
    qDebug() << "ADD PLAYBACK";
    QString url = "file:///C:/1/1.avi";
    //global_manager->addPlayback(url.toLocal8Bit().data());

}


void CScenePanel::restoreStateFromFile(QString fname)
{
    QFile file(fname);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QJsonDocument doc = QJsonDocument::fromJson( file.readAll() );
    restoreState( doc.object() );

    file.close();
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


