#include "menubarwidget.h"
#include "startairdialog.h"
#include "startrecorddialog.h"

#include <QDebug>
#include <QHBoxLayout>

#define STAT_PERIOD 3
#define START_AIR "Start Air"
#define START_RECORD "Start Record"

#define GREEN_ICO ":green.ico"
#define RED_ICO ":red.ico"

MenuBarWidget::MenuBarWidget(QWidget *parent) :
    QWidget(parent)
{    
    QHBoxLayout *lay= new QHBoxLayout(this);
    lay->setSpacing(6);
    lay->setContentsMargins(0, 0, 0, 0);


    m_startAirBtn = new QPushButton(START_AIR, this); // TODO:"9999 FPS 9999 Kbs"
    m_startAirBtn->setFlat(true);
    m_startAirBtn->setIcon(QIcon(GREEN_ICO));
    connect(m_startAirBtn, SIGNAL(clicked()), SLOT(on_startAirBtn_clicked()));
    lay->addWidget(m_startAirBtn);

    /*
    m_startRecordBtn = new QPushButton(START_RECORD, this);
    m_startRecordBtn->setFlat(true);
    m_startRecordBtn->setIcon(QIcon(GREEN_ICO));
    lay->addWidget(m_startRecordBtn);
    */

    air_timer = new QTimer(this);
    connect(air_timer, SIGNAL(timeout()), this, SLOT(updateAirStat()));

    m_startAirBtn->setMinimumWidth(150);
}

MenuBarWidget::~MenuBarWidget()
{
    //delete ui;
}

void MenuBarWidget::on_startAirBtn_clicked()
{    
    if(!air_timer->isActive())
    {
        StartAirDialog * startAirDialog = new StartAirDialog(this);
        startAirDialog->setAttribute(Qt::WA_DeleteOnClose);
        if(startAirDialog->exec() == QDialog::Accepted)
        {
            airStarting();
        }
    }
    else
    {
        global_manager->stopAir();
        airStoping();
    }

}

void MenuBarWidget::recordStarting()
{
    //ui->startRecordBtn->setState(MenuIndicator::Progress);
}

void MenuBarWidget::recordStoping()
{
    //ui->startRecordBtn->setState(MenuIndicator::Idle);
}

void MenuBarWidget::airStarting()
{   
    air_timer->start(STAT_PERIOD*1000);
    m_total_bytes = 0;
    m_total_frames = 0;
    m_startAirBtn->setIcon(QIcon(RED_ICO));

    updateAirStat();
}

void MenuBarWidget::airStoping()
{    
    air_timer->stop();    
    m_startAirBtn->setIcon(QIcon(GREEN_ICO));
    m_startAirBtn->setText(START_AIR);
}


void MenuBarWidget::updateAirStat()
{
    static char _air_stat[255];
    QStringList parts;
    uint64 total_bytes, total_frames;
    int fps, br, traff;

    global_manager->getAirStat(&total_bytes, &total_frames); // возвращает строку TOTAL_BYTES:TOTAL_FRAMES

    // вычисляем битрейт в килобитах/сек: 8*(кол-во поступивших байт)/(1024*кол-во секунд в периоде)
    br = ((float)(total_bytes - m_total_bytes))/(128*STAT_PERIOD);

    // TODO
    // фиксируем максимальный битрейт
    /*
        if( bitrate > m_MaxBitrate )
            m_MaxBitrate = bitrate;
    */

    // вычисляем fps
    fps = (float)(total_frames - m_total_frames)/STAT_PERIOD;

    qDebug() << "UPDATE AIR STAT " << total_bytes << total_frames;

    //startAirBtn->setIndicatorText(QString("%1 fps, %2 Kbs").arg(fps).arg(br));
    m_startAirBtn->setText(QString("%1 fps, %2 kbs").arg(fps).arg(br));
    m_total_bytes = total_bytes;
    m_total_frames = total_frames;
}

