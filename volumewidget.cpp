#include "volumewidget.h"

#include <QLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>

#include <QDebug>

CVolumeWidget::CVolumeWidget(QWidget *parent) :
    QWidget(parent),
    _volume(50)
{
    init();
}

CVolumeWidget::CVolumeWidget(qint32 volume, QWidget *parent) :
    QWidget(parent),
    _volume(volume)
{
    Q_UNUSED(volume);
    
    init();
}

void CVolumeWidget::init()
{
    _isMute = true;

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(6);
    layout->setContentsMargins(0, 0, 0, 0);

    QPushButton *pbMute = new QPushButton(this);
    pbMute->setProperty("mute", "off");
    pbMute->setObjectName("pbMute");
    pbMute->setStyleSheet(QString("#pbMute { border-image: url(resources/mute_off.png); background-color: transparent; max-width: 150px; max-height: 150px; margin-top: 0px; margin-left: 0px; margin-right: 0px;}"));

    connect(pbMute, SIGNAL(clicked()), SLOT(onPbMuteClicked()));
    layout->addWidget(pbMute);
    
    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(0);
    lSliderName = new QLabel("Empty", this);
    lSliderName->setAlignment(Qt::AlignCenter);

    verticalLayout->addWidget(lSliderName);
    
    QString sliderStyle = "#volumeSlider::groove:horizontal { \
        border: 1px solid #999999; \
        height: 8px; /* the groove expands to the size of the slider by default. by giving it a height, it has a fixed size */ \
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4); \
        margin: 2px 0;\
    }\
    \
    #volumeSlider::handle:horizontal {\
        background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);\
        border: 1px solid #5c5c5c;\
        width: 18px;\
        margin: -2px 0; /* handle is placed by default on the contents rect of the groove. Expand outside the groove */\
        border-radius: 3px;\
    }";
    QSlider *slider = new QSlider(this);
    slider->setObjectName("volumeSlider");
    slider->setStyleSheet(sliderStyle);
    slider->setOrientation(Qt::Horizontal);
    slider->setTickPosition(QSlider::TicksAbove);
    slider->setTickInterval(1);
    slider->setMaximum(100);
    slider->setValue(_volume);
    connect(slider, SIGNAL(valueChanged(int)), SLOT(onSliderValueChanged(int)));    

    verticalLayout->addWidget(slider);
    layout->addLayout(verticalLayout);
}

void CVolumeWidget::onPbMuteClicked()
{
    if(QPushButton * pb = dynamic_cast<QPushButton *>(sender()))
    {
        if(pb->property("mute").toString().compare("off") == 0)
            pb->setProperty("mute", "on");
        else
            pb->setProperty("mute", "off");
        
        pb->setStyleSheet(QString("#pbMute { border-image: url(resources/mute_%1.png); background-color: transparent; max-width: 150px; max-height: 150px; margin-top: 0px; margin-left: 0px; margin-right: 0px;}").arg(pb->property("mute").toString()));
        qDebug() << "mute: " << pb->property("mute").toString();
    }

}

void CVolumeWidget::onSliderValueChanged(int value)
{
    _volume = value;
    qDebug() << "Volume: " << _volume;
}

void CVolumeWidget::setText(const QString &text)
{
    lSliderName->setText(text);
}

void CVolumeWidget::setVolume(qint32 volume)
{
    onSliderValueChanged(volume);
}

qint32 CVolumeWidget::volume() const
{
    return _volume;
}

void CVolumeWidget::setMute(bool mute)
{
    _isMute = mute;
}

bool CVolumeWidget::isMute() const
{
    return _isMute;
}