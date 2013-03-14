#include "scenepanel.h"

#include <QLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QDebug>

#include "IManager.h"

CScenePanel::CScenePanel(qint32 compkey, QWidget *parent) :
    QWidget(parent)
{
    _sceneWidget = new CSceneWidget(compkey, this);
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

    if(sourceName.startsWith("SUBSCENE")){
        layer_compkey = global_manager->addScene();
    }else{
        layer_compkey = global_manager->addLayer(_sceneWidget->getCompkey(), sourceName.toLocal8Bit().data(), zorder);
    }

    CLayerWidget *lw = new CLayerWidget(layer_compkey, this);
    _listLayerWidgets.append(lw);
    rePosition();

    return lw;
}



void CScenePanel::onPbAddPreviewWidget()
{
    _sceneWidget->showBox(1);
}

void CScenePanel::onPbApply()
{
    qDebug() << _sceneWidget->apply();
}

void CScenePanel::resizeEvent(QResizeEvent *event)
{
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

    _sceneWidget->setGeometry(0, 0, w/2, h);

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
