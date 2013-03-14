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
#ifdef Q_OS_WIN32
    layer_compkey = global_manager->addLayer(_sceneWidget->getCompkey(), sourceName.toLocal8Bit().data(), zorder);
#endif
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
    int w,h, sw, sh, sx, sy, zw, zh;
    w = this->width();
    h = this->height();

    sx = w/2;
    sy = 0;

    // параметры сетки
    zh = 7; // высота зазора между строками
    zw = 7; // ширина зазора между столбцами
    sw = (w/2 - 2*zw)/3; // три столбца и два зазора
    sh = (h - 2*zh)/3; // три ряда, высотой sh с двумя зазорами

    _sceneWidget->setGeometry(0, 0, w/2, h);

    for(int i=0; i<_listLayerWidgets.size(); i++){
        if( i>0 && i % 3 == 0 ){
            sy += sh + zh;
            sx = w/2;
        }
        // вставка зазора между столбцами
        if(i % 3 != 0){
            sx += zw;
        }

        _listLayerWidgets[i]->setGeometry(sx, sy, sw, sh);
        _listLayerWidgets[i]->show();
        sx += sw;
    }

}
