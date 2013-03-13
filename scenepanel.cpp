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

void CScenePanel::addLayer(const QString &sourceName)
{
    int zorder = 10*(_listLayerWidgets.count() + 1); // в микшер слои добавляем поверх друг друга
    int layer_compkey;
    layer_compkey = global_manager->addLayer(_sceneWidget->getCompkey(), sourceName.toLocal8Bit().data(), zorder);
    CLayerWidget *lw = new CLayerWidget(layer_compkey, this);
    _listLayerWidgets.append(lw);
    rePosition();
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
    // делим родительскую ширину между сценой и сеткой слоёв
    // и высоту сетки делим между слоями.
    int w,h, sw, sh, sx, sy;
    w = this->width();
    h = this->height();

    sx = w/2;
    sy = 0;

    sw = (w/2)/3; // три столбца
    sh = h/3; // три ряда, высотой sh

    _sceneWidget->setGeometry(0, 0, w/2, h);

    for(int i=0; i<_listLayerWidgets.size(); i++){
        if( i>0 && i % 3 == 0 ){
            sy += sh;
            sx = w/2;
        }
        _listLayerWidgets[i]->setGeometry(sx, sy, sw, sh);
        _listLayerWidgets[i]->show();
        sx += sw;
    }

}
