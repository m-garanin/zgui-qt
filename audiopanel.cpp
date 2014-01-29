#include "audiopanel.h"
#include "utils.h"
#include "IManager.h"

#include <QLayout>
#include <QJsonArray>

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

CVolumeWidget* CAudioPanel::addAudio(QString src)
{
    CVolumeWidget *vw = NULL;

    if(global_manager->addAudioSource(src.toLocal8Bit().data()))
    {    
        vw = new CVolumeWidget(src, this);     
        addVolumeWidget(vw);
    }

    return vw;
}

void CAudioPanel::addVolumeWidget(CVolumeWidget *volumeWidget)
{    
    _mainLayout->removeItem(_verticalSpacer);
    _mainLayout->addWidget(volumeWidget);
    _mainLayout->addItem(_verticalSpacer);

    m_vs.append(volumeWidget);

    if(!m_timer->isActive() && m_vs.length() > 1){
        // если только мастер, то смысла стартовать нет,
        // к тому же возникает странная проблема (скорей всего система не успевает поднять мастер)
        m_timer->start(LEVELS_UPDATE_PERIOD);
    }

    // когда источников становиться два и более, отображааем master-звук
    if(m_vs.length() > 2){
        m_master->setHidden(false);
    }

    connect(volumeWidget, SIGNAL(deleteAudio()), SLOT(onDeleteAudio()));

}

QJsonObject CAudioPanel::saveState()
{
    // NOTES: master-audio не сохраняем

    QJsonArray arr;
    CVolumeWidget* vw;
    QString source_id;
    QListIterator<CVolumeWidget*> it(m_vs);

    while(it.hasNext())
    {
        QJsonObject obj;
        vw = it.next();
        source_id = vw->getPersistentSourceId();

        if(source_id == "MASTER") continue;

        obj.insert("source_id", source_id);
        obj.insert("mute", vw->getMute() );
        obj.insert("volume", vw->volume() );

        arr.append(obj);
    }

    QJsonObject mobj;
    mobj.insert("items", arr);

    return mobj;
}

void CAudioPanel::restoreState(QJsonObject mobj)
{    
    QJsonArray arr;
    QJsonValue v;
    QJsonObject obj;
    QString source_id;
    qreal vol;
    bool is_mute;
    CVolumeWidget *vw;
    QStringList devs = getAudioCaptureDevices();

    arr = mobj.take("items").toArray();
    for(int i=0;i<arr.size();i++){
        obj = arr.at(i).toObject();

        source_id = obj.value("source_id").toString();

        // проверка что девайс существует на момент восстановления
        if(!devs.contains(source_id)) continue;


        is_mute = obj.value("mute").toBool();
        vol = obj.value("volume").toDouble();

        vw = addAudio(source_id);
        if(vw != NULL){
            vw->setVolume(vol);
            vw->setMute(is_mute);
        }
    }


}

void CAudioPanel::updateLevels()
{    
    int size;
    global_manager->getAudioLevels(&levels[0], &size);

    for(int i=0;i<size;i++){
        m_vs[i]->setLevelDb(levels[i]);        
    }
}

void CAudioPanel::onDeleteAudio()
{    
    CVolumeWidget* vw = qobject_cast<CVolumeWidget*>(sender());
    vw->setMute(true);
    vw->hide();    
    global_manager->deleteAudioSource(vw->getPersistentSourceId().toLocal8Bit().data());
    m_vs.removeOne(vw);

    // при условии скрываем master-звук
    if(m_vs.length() < 3){
        m_master->setHidden(true);
    }

}

void CAudioPanel::resizeEvent(QResizeEvent *event)
{
    this->setGeometry(parentWidget()->width()*.25,0, parentWidget()->width()*0.5, this->height());
}
