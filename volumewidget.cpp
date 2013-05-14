#include "volumewidget.h"

#include <QLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QDebug>
#include <QProgressBar>

#include "ui_volumewidget.h"
#include "IManager.h"

CVolumeWidget::CVolumeWidget(const QString &sourceKey, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VolumeWidget),
    _sourceKey(sourceKey)
{
    ui->setupUi(this);
    ui->label->setText(sourceKey);
    _volume = 1;
    setObjectName("Volume");
    if(sourceKey == "MASTER"){
        ui->label->setText(tr("MASTER AUDIO"));
    }
    init();
    setAttribute(Qt::WA_StyledBackground, true);
}

void CVolumeWidget::init()
{
    QPushButton *pbMute = ui->pbMute;
    pbMute->setProperty("mute", "off");
    pbMute->setObjectName("pbMute");
    pbMute->setIconSize(QSize(32,32));
    //pbMute->setIcon(QIcon(":/images/mute_on.png"));
    pbMute->setStyleSheet(QString("#pbMute { border-image: url(:/images/mute_off.png); background-color: transparent; max-width: 150px; max-height: 150px; margin-top: 0px; margin-left: 0px; margin-right: 0px;}"));

    connect(pbMute, SIGNAL(clicked()), SLOT(onPbMuteClicked()));

    ui->pbar->setMaximumHeight(8);

    QSlider *slider = ui->slider;
    slider->setTickInterval(1);
    slider->setMaximum(10);
    connect(slider, SIGNAL(valueChanged(int)), SLOT(onSliderValueChanged(int)));

    slider->setValue(_volume);

    //
    ui->pbar->setRange(0,100);

}

void CVolumeWidget::onPbMuteClicked()
{
    if(QPushButton * pb = dynamic_cast<QPushButton *>(sender()))
    {
        if(pb->property("mute").toString().compare("off") == 0){
            pb->setProperty("mute", "on");
            global_manager->mute(_sourceKey.toLocal8Bit().data());
        }
        else{
            pb->setProperty("mute", "off");
            global_manager->unmute(_sourceKey.toLocal8Bit().data());
        }
        
        pb->setStyleSheet(QString("#pbMute { border-image: url(:/images/mute_%1.png); background-color: transparent; max-width: 150px; max-height: 150px; margin-top: 0px; margin-left: 0px; margin-right: 0px;}").arg(pb->property("mute").toString()));
    }

}

void CVolumeWidget::onSliderValueChanged(int value)
{
    _volume = value;
    global_manager->setVolume(_sourceKey.toLocal8Bit().data(), _volume);
    qDebug() << "Volume: " << _volume;
}


void CVolumeWidget::setVolume(qreal volume)
{
    onSliderValueChanged(volume);
}

qreal CVolumeWidget::volume() const
{
    return _volume;
}


void CVolumeWidget::setLevelDb(double val)
{
    //if(val < ui->pbar->minimum())
    //    val = ui->pbar->minimum();

    val = 100*pow(10, val/20);
    if(val < ui->pbar->minimum() ){
        val = ui->pbar->minimum();
    }else{
        if(val > ui->pbar->maximum() )
            val = ui->pbar->maximum();
    }

    ui->pbar->setValue(val);

}
