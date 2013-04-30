#include "menubarwidget.h"
#include "ui_menubarwidget.h"
#include <QDebug>

#include "IManager.h"

MenuBarWidget::MenuBarWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MenuBarWidget)
{
    ui->setupUi(this);
    air_timer = new QTimer(this);
    connect(air_timer, SIGNAL(timeout()), this, SLOT(updateAirStat()));

}

MenuBarWidget::~MenuBarWidget()
{
    delete ui;
}

void MenuBarWidget::on_startRecordBtn_clicked()
{
    emit startRecordBtnClicked(ui->startRecordBtn->getState());
}

void MenuBarWidget::on_startAirBtn_clicked()
{
    emit startAirBtnClicked(ui->startAirBtn->getState());
}

void MenuBarWidget::recordStarting()
{
    ui->startRecordBtn->setState(MenuIndicator::Progress);
}

void MenuBarWidget::recordStoping()
{
    ui->startRecordBtn->setState(MenuIndicator::Idle);
}

void MenuBarWidget::airStarting()
{
    ui->startAirBtn->setState(MenuIndicator::Progress);
    air_timer->start(3*1000);
    updateAirStat();
}

void MenuBarWidget::airStoping()
{
    air_timer->stop();
    ui->startAirBtn->setState(MenuIndicator::Idle);

}

void MenuBarWidget::resizeWidget()
{
    //this->resize(ui->startRecordBtn->width() + ui->startAirBtn->width(), this->height());
    //this->updateGeometry();
}


void MenuBarWidget::updateAirStat()
{
    static char _air_stat[255];
    QStringList parts;
    int fps, br, traff;

    global_manager->getAirStat(_air_stat); // возвращает строку TOTAL_BYTES:TOTAL_FRAMES

    // TODO
    /*
    ULONG total_bytes;
        ULONG total_frames;

        this->m_Streamer.m_pMuxer->GetStatistic(&total_bytes, &total_frames);

        // вычисляем битрейт в килобитах/сек: 8*(кол-во поступивших байт)/(1024*кол-во секунд в периоде)
        int bitrate = ((float)(total_bytes - m_Bytes))/(128*STAT_PERIOD);
        // фиксируем максимальный битрейт
        if( bitrate > m_MaxBitrate )
            m_MaxBitrate = bitrate;


        // вычисляем fps
        int fps = (float)(total_frames - m_Frames)/STAT_PERIOD;

        // общее число секунд
        m_Dur += STAT_PERIOD;
        m_Bytes = total_bytes;
        m_Frames = total_frames;
     */

    parts = QString(_air_stat).split(":");

    fps = parts[0].toInt();
    br = parts[1].toInt();
    traff = parts[2].toInt();


    qDebug() << "UPDATE AIR STAT " << fps << br << traff;
    ui->startAirBtn->setIndicatorText(QString("%1 fps, %2 Kbs").arg(fps).arg(br));
}
