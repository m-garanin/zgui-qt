#include "audiopanel.h"
#include "IManager.h"

#include <QLayout>

CAudioPanel::CAudioPanel(QWidget *parent) :
    QWidget(parent)
{
    _mainLayout = new QVBoxLayout(this);
    _mainLayout->setSpacing(0);
    _mainLayout->setContentsMargins(11, 11, 11, 11);

    _verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    setObjectName("AudioPanel");
    setAttribute(Qt::WA_StyledBackground, true);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateLevels()));

    //
    m_master = new CVolumeWidget("MASTER", this);
    addVolumeWidget(m_master);
    m_master->setHidden(true);
}

void CAudioPanel::addVolumeWidget(CVolumeWidget *volumeWidget)
{    
    _mainLayout->removeItem(_verticalSpacer);
    _mainLayout->addWidget(volumeWidget);
    _mainLayout->addItem(_verticalSpacer);

    m_vs.append(volumeWidget);

    if(!m_timer->isActive()){
        m_timer->start(LEVELS_UPDATE_PERIOD);
    }

    // когда источников становиться два и более, отображааем master-звук
    if(m_vs.length() > 2){
        m_master->setHidden(false);
    }
}

void CAudioPanel::updateLevels()
{
    int size;
    global_manager->getAudioLevels(&levels[0], &size);
    //qDebug() << "updateLevels " << size << levels[size-1];

    for(int i=0;i<size;i++){
        m_vs[i]->setLevelDb(levels[i]);
    }
}

void CAudioPanel::resizeEvent(QResizeEvent *event)
{
    this->setGeometry(parentWidget()->width()*.25,0, parentWidget()->width()*0.5, this->height());
}
