#include <QDebug>
#include <QFileDialog>
#include <QStringList>
#include <QScrollArea>
#include <QPushButton>
#include <QTimer>
#include <QTextStream>
#include <QUrl>
#include <QMessageBox>
#include <QDateTime>
#include <QAudioDeviceInfo>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "volumewidget.h"
#include "audiopanel.h"
#include "scenepanel.h"


#include "IManager.h"

#include "settingsdlg.h"

#include "rectselectionwidget.h"

#include "settingsmanager.h"
#include "startairdialog.h"
#include "startrecorddialog.h"
#include "captureselectdialog.h"
#include "utils.h"
#include "zcore.h"

IManager* global_manager;
typedef void (__cdecl *ZCORE_GET_GLOBAL_MANAGER)(IManager**);
MainWindow* g_main_window;

#define STAT_PERIOD 3

void app_logger(char* buf){
    g_main_window->logger(buf);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    g_main_window = this;
    SettingsManager::setGlobalSettingsFilePath(QDir::homePath() + "/zgui.ini");
    m_logfile = new QFile(QDir::homePath() + "/zgui_debug.txt");
    m_logfile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Unbuffered);
    // логируем старт
    log_fixstart();


    ui->setupUi(this);

    // statusbar timer
    m_statusbar_timer = new QTimer(this);
    m_statusbar_timer->start(500);
    connect(m_statusbar_timer, SIGNAL(timeout()), this, SLOT(statusBarTimerTimeout()));


    detectVersion();

    loadSplitterSettings();
    start();


    _audioPanel = new CAudioPanel(this);
    ui->bottom->addWidget(_audioPanel);

    //
    QToolBar* tbar = ui->mainToolBar;    
    tbar->setIconSize(QSize(48,48));
    //tbar->setOrientation(Qt::Horizontal);

    // cam  (обработку отдаём в ScenePanel)
    connect(tbar->addAction(QIcon(":cam"), tr("Add camera")),
            &QAction::triggered, _scenePanel, &CScenePanel::onVideoCaptureSelect);

    // images (обработку отдаём в ScenePanel)
    connect(tbar->addAction(QIcon(":img"), tr("Add image")),
            &QAction::triggered, _scenePanel, &CScenePanel::onImageSelect );

    // screen-capture (обработку отдаём в ScenePanel)
    connect(tbar->addAction(QIcon(":screen_capture"), tr("Add screen capture")),
            &QAction::triggered, _scenePanel, &CScenePanel::onAddScreenCapture );

    // video file
    connect(tbar->addAction(QIcon(":video"), tr("Add video file") ),
            &QAction::triggered, _scenePanel, &CScenePanel::onVideoFileSelect);

    // для Professionl Mode
    if(getProfMode()){
        // html plugins (обработку отдаём в ScenePanel)
        connect(tbar->addAction(QIcon(":html_plugins"), tr("Add plugins")),
                &QAction::triggered, _scenePanel, &CScenePanel::onAddHtmlRender);

        // subscene
        connect(tbar->addAction(QIcon(":scene"), tr("Add sub-scene")),
                &QAction::triggered, this, &MainWindow::on_menusubscene_triggered);

    }


    // net-source
    connect(tbar->addAction(QIcon(":netsource"), tr("Add network source") ),
            &QAction::triggered, _scenePanel, &CScenePanel::onNetSourceSelect);

    // webpage-source
    connect(tbar->addAction(QIcon(":netsource"), tr("Add webpage") ),
            &QAction::triggered, _scenePanel, &CScenePanel::onWebPageSelect);


    // sound
    connect(tbar->addAction(QIcon(":mic"), tr("Add sound device")),
            &QAction::triggered, this, &MainWindow::onAudioCaptureSelect);

    // settings
    connect(tbar->addAction(QIcon(":settings"), tr("Settings")),
            SIGNAL(triggered()), SLOT(onActionSettingsTriggered()));




    // фейк для занятия места
    QWidget *spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    spacerWidget->setVisible(true);
    tbar->addWidget(spacerWidget);

    // rec
    m_rec = new QToolButton(this);
    m_rec->setIcon(QIcon(":rec_wait"));
    tbar->addWidget(m_rec);
    connect(m_rec,
           SIGNAL(clicked()), SLOT(onRecTriggered()));


    // air
    m_air = new AirWidget(this);
    tbar->addWidget(m_air);
    connect(m_air,
           SIGNAL(clicked()), SLOT(onAirTriggered()));

    // live-stat button
    QWidget *tmp = new QWidget(this);
    tmp->setSizePolicy(QSizePolicy::Expanding , QSizePolicy::Preferred);
    m_air_info = new QToolButton();
    m_air_info->setObjectName("LiveStatus");
    m_air_info->setToolTip(tr("FPS / BITRATE. Click for open."));
    connect(m_air_info,
           SIGNAL(clicked()), SLOT(onAirInfoTriggered()) );
    m_air_info->setText(" ");
    m_air_info->setDisabled(true);

    tbar->addWidget(m_air_info);

    // восстанавливаем последнее состояние
    restoreLastConfig();    
}

MainWindow::~MainWindow()
{
    saveLastConfig();
    global_manager->stopPipeline();
    saveSplitterSettings();    
    delete ui;
}

void MainWindow::loadSplitterSettings()
{
    QList<int> splitterList;
    SettingsManager settings("MainWindow");
    QString splitter = settings.getValue("splitter").toString();

    QStringList strSplitterList = splitter.split(":");

    for(int i = 0; i < strSplitterList.size(); ++i){
        splitterList.push_back(strSplitterList.at(i).toInt());
    }

    if(splitter.isEmpty()) // if empty, set default size
    {
        splitterList.clear();
        splitterList << size().height() * 0.7 << size().height() * 0.3;
    }

    ui->splitter->setSizes(splitterList);
}

void MainWindow::saveSplitterSettings()
{
    SettingsManager settings("MainWindow");
    QString splitter;
    foreach (int size, ui->splitter->sizes()) {
        splitter += QString("%1").arg(size);
        splitter += ":";
    }
    splitter.remove(splitter.length() - 1, 1);
    settings.setValue("splitter", splitter);
}


void MainWindow::start()
{    
    //
    // загрузка функции из zcore.dll
    m_zcoreLib.setFileName("zcore.dll");

    bool r = m_zcoreLib.load();
    qDebug() << "LOAD ZCORE LIBRARY" << r;
    if (!r) {
       //qDebug() << m_zcoreLib.errorString();
        QMessageBox::critical(this, "ERROR LOAD ZCORE.dll", m_zcoreLib.errorString() ) ;
    }

    qDebug() << "s 0" ;
    ZCORE_GET_GLOBAL_MANAGER ggm =(ZCORE_GET_GLOBAL_MANAGER)m_zcoreLib.resolve("getGlobalManager");
    ggm(&global_manager);
    qDebug() << "s 10" << ggm;
    // получаем размеры рабочей области
    SettingsManager setting("Settings");
    QString wsize = getWorksize();

    QStringList sz = wsize.split("x");
    uint w = sz[0].toInt();
    uint h = sz[1].toInt();

    // проверка ключа
    char* bufkey;
    int chk_key = global_manager->checkKey(bufkey);
    qDebug() << "s 20" << chk_key;

    if(chk_key == 0){
        QMessageBox::critical(this,"Licence key", "This licence key expired");        
        return;
    }

    if(chk_key == 2){
        QMessageBox::warning(this,"Licence key", "This licence key will soon expire");
    }

    // проверка, что присутствует аудио-выход
    QAudioDeviceInfo adev = QAudioDeviceInfo::defaultOutputDevice();
    if(adev.isNull()){
        QMessageBox::critical(this, "AUDIO ERROR", "Not found audio output");
        return;
    }

    qDebug() << "s 30";
    //
    global_manager->startPipeline(w, h, (app_logger_callback)app_logger );
    qDebug() << "s 40";
    _scenePanel = new CScenePanel(100, this);
    ui->top->addWidget(_scenePanel);

    //
    int ptr = setting.getIntValue("Workpattern");
    global_manager->setBackground(ptr);

    // таймер для air-статистики
    air_timer = new QTimer(this);
    connect(air_timer, SIGNAL(timeout()), this, SLOT(updateAirStat()));

    // таймер для rec-статистики
    rec_timer = new QTimer(this);
    connect(rec_timer, SIGNAL(timeout()), this, SLOT(updateRecStat()));

}

void MainWindow::log_fixstart()
{
    QDateTime t = QDateTime::currentDateTime();
    QString s = "\r\n------- START:" + t.toString("dd.MM.yy hh:mm:ss");
    logger(s.toLocal8Bit().data());
}

void MainWindow::detectVersion()
{
    QFile v(qApp->applicationDirPath() +"/VERSION.txt");
    v.open(QIODevice::ReadOnly);
    QByteArray data = v.readAll();
    QString title = "ZGUI - " + QString(data);

    if(getProfMode()){
        title += tr(" PROFESSIONAL Mode");
    }else{
        title += tr(" LITE Mode");
    }

    setWindowTitle(title);
}

void MainWindow::logger(char *buf)
{
    QString msg = QString(buf);

    m_logfile->write(buf);
    m_logfile->write("\r\n");

    qDebug() << "LOGGER:" << msg;
    if(msg.startsWith("ERROR_DEVICE")){
        msg = QString("WARNING: ") + msg.split(":").at(1);
        setStatusBarMessage(msg);
    }


}

void MainWindow::onAudioCaptureSelect()
{    
    CaptureSelectDialog dlg(CaptureSelectDialog::AudioDevice);
    if(dlg.exec() == QDialog::Accepted){        
        _audioPanel->addAudio(dlg.getDevice());
    }
}

void MainWindow::on_menusubscene_triggered()
{
    _scenePanel->addSubSceneLayer();
}



void MainWindow::onActionSettingsTriggered()
{
    CSettingsDlg dlg;
    dlg.exec();
}

void MainWindow::onAirTriggered()
{
    if(!air_timer->isActive())
    {
        StartAirDialog * dlg = new StartAirDialog(this);
        //dlg->setAttribute(Qt::WA_DeleteOnClose);
        if(dlg->exec() == QDialog::Accepted)
        {            
            m_total_tick = 0;
            m_total_bytes = 0;
            m_total_frames = 0;
            air_timer->start(STAT_PERIOD*1000);
            m_air->setOnAir(dlg->test_mode);
            updateAirStat();            
            m_air_info->setDisabled(false);
        }
    }
    else
    {
        air_timer->stop();
        m_air->setStop();
        global_manager->stopAir();

        int abr, afps;
        // вычисляем битрейт в килобитах/сек: 8*(кол-во поступивших байт)/(1024*кол-во секунд в периоде)
        abr = (float)m_total_bytes/(128*m_total_tick*STAT_PERIOD);
        afps = (float)m_total_frames/(m_total_tick*STAT_PERIOD);
        m_air_info->setText(QString("%1 / %2").arg(afps).arg(abr));

    }

}

void MainWindow::onAirInfoTriggered()
{
    this->m_big_air_info.show();
}

void MainWindow::updateAirStat()
{
    static char _air_stat[255];
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

    //qDebug() << "UPDATE AIR STAT " << total_bytes << total_frames;

    m_air_info->setText(QString("%1 / %2").arg(fps).arg(br));
    m_big_air_info.updateStat(fps, br);

    m_total_bytes = total_bytes;
    m_total_frames = total_frames;
    if(total_bytes > 0)
        m_total_tick ++;
}

void MainWindow::onRecTriggered()
{
    if(!rec_timer->isActive())
    {
        StartRecordDialog * dlg = new StartRecordDialog(this);        
        if(dlg->exec() == QDialog::Accepted)
        {            
            rec_timer->start(STAT_PERIOD*1000);
            //m_total_bytes = 0;
            //m_total_frames = 0;
            //m_air->setOnAir(dlg->test_mode);
            updateRecStat();
            //m_rec_info->setDisabled(false);
            m_rec->setIcon(QIcon(":rec"));
        }
    }
    else
    {
        rec_timer->stop();
        m_rec->setIcon(QIcon(":rec_wait"));
        //m_air->setStop();
        //m_air_info->setText(" ");
        //m_air_info->setDisabled(true);
        global_manager->stopRec();
    }

}

void MainWindow::updateRecStat()
{
    //
}

void MainWindow::statusBarTimerTimeout()
{ 
    if(m_statusbar_msg.isEmpty()) return;

    m_statusbar_mutex.lock();
    statusBar()->showMessage(m_statusbar_msg, 10000);
    m_statusbar_msg.clear();
    m_statusbar_mutex.unlock();
}

#define SCENE_CONFIG_FNAME QDir::homePath() + "/zgui_last_scene.txt"

void MainWindow::saveLastConfig()
{
    if( getAutoSaveRestore()){
        _scenePanel->saveStateToFile(SCENE_CONFIG_FNAME);
    }
}

void MainWindow::restoreLastConfig()
{
    if( getAutoSaveRestore()){
        if(QFile::exists(SCENE_CONFIG_FNAME)){
            _scenePanel->restoreStateFromFile(SCENE_CONFIG_FNAME);}
    }
}

void MainWindow::setStatusBarMessage(QString msg)
{
    m_statusbar_mutex.lock();
    m_statusbar_msg = msg;
    m_statusbar_mutex.unlock();
}



