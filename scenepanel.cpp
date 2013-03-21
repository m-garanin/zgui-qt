#include "scenepanel.h"

#include <QLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QDebug>

#include "IManager.h"

CScenePanel::CScenePanel(qint32 compkey, QWidget *parent) :
    QWidget(parent)
{
    _graphicsView = new CGraphicsView(compkey, 640, 360, this);
}

void CScenePanel::addCamLayer(const QString &sourceName)
{
    addLayer("CAM://" + sourceName);
}

void CScenePanel::addImageLayer(QString fname)
{
    CLayerWidget *lw;
    lw = addLayer("IMAGE://" + fname);
    // TODO: установка флага что это image
}

void CScenePanel::addHtmlRenderLayer(const QString &url)
{
    qDebug() << "CScenePanel::addHtmlRenderLayer: url: " << url;
    addLayer("HTML://" + url);
}


void CScenePanel::addSubSceneLayer()
{
    CLayerWidget *lw;
    lw = addLayer("SUBSCENE://");
    // TODO: установка флага что это подсцена
}

CLayerWidget* CScenePanel::addLayer(const QString &sourceName)
{
    int zorder = 10*(_listLayerWidgets.count() + 1); // в микшер слои добавляем поверх друг друга
    int layer_compkey;

    CLayerWidget::LayerType lType = CLayerWidget::ELayerTypeDefault;

    if(sourceName.startsWith("SUBSCENE")){
        lType = CLayerWidget::ELayerTypeSUBSCENE;
        layer_compkey = global_manager->addScene();
    }else{
        layer_compkey = global_manager->addLayer(_graphicsView->getCompkey(), sourceName.toLocal8Bit().data(), zorder);
    }

    CLayerWidget *lw = new CLayerWidget(layer_compkey, lType, this);
    connect(lw, SIGNAL(editLayer(qint32)), SLOT(onEditLayer(qint32)));
    connect(lw, SIGNAL(ultimateShow()), SLOT(onUltimateShow()));
    _listLayerWidgets.append(lw);
    rePosition();

    return lw;
}

void CScenePanel::onEditLayer(qint32 compkey)
{
    _graphicsView->showBox(compkey);
}

void CScenePanel::onUltimateShow()
{
    if(CLayerWidget *curLW = qobject_cast<CLayerWidget*>(sender()))
    {
        QListIterator<CLayerWidget*> it(_listLayerWidgets);

        while (it.hasNext())
        {
            CLayerWidget *lw = it.next();
            
            if(curLW->compKey() == lw->compKey())
            {
                lw->setVisibleHide(true);
                continue;
            }

            if(lw->isPinEnable() || !lw->isVisibleHide())
                continue;

            lw->setVisibleHide(false);
        }
    }
}

void CScenePanel::onPbAddPreviewWidget()
{
    _graphicsView->showBox(1);
}

void CScenePanel::onPbApply()
{
    qDebug() << _graphicsView->apply();
}

void CScenePanel::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
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
    zh = 7; // высота зазора между строками
    zw = 7; // ширина зазора между столбцами
    sw = (w/2 - (cols-1)*zw)/cols; // ширина ячейки с учётом зазоров
    sh = (h - (rows-1)*zh)/rows; //  высота ячейки с учётом зазоров

    sx = w/2;
    sy = 0;

//    _sceneWidget->setGeometry(0, 0, w/2, h);
        // TODO: fixme
//    qDebug() << (w/2)/rectView.width() << "x" << h/rectView.height();
    //QRect rectView = _graphicsView->geometry();
    //_graphicsView->scale((w/2)/rectView.width(), h/rectView.height());
    //_graphicsView->scale(0.9, 0.9);
    //qDebug() << (w/2)/rectView.width() << "x" << h/rectView.height();
    _graphicsView->setGeometry(0, 0, w/2, h);
    //_graphicsView->setSceneRect(0,0,w/2,h);

    for(int i=0; i<_listLayerWidgets.size(); i++){
        if( i>0 && i % cols == 0 ){
            sy += sh + zh;
            sx = w/2;
        }

        sx += zw; // вставка зазора между столбцами

        _listLayerWidgets[i]->setGeometry(sx, sy, sw, sh);
        _listLayerWidgets[i]->show();
        sx += sw;
    }

}

void CScenePanel::start()
{
    _graphicsView->start();
    _graphicsView->startBox();

    QListIterator<CLayerWidget*> it(_listLayerWidgets);

    while(it.hasNext())
    {
        it.next()->start();
    }
}

void CScenePanel::stop()
{
    _graphicsView->stop();
    _graphicsView->stopBox();

    QListIterator<CLayerWidget*> it(_listLayerWidgets);

    while(it.hasNext())
    {
        it.next()->stop();
    }
}








