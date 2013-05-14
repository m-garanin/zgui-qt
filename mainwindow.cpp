#include <QDebug>
#include <QFileDialog>
#include <QStringList>
#include <QScrollArea>
#include <QPushButton>
#include <QTimer>
#include <QTextStream>

#include <QUrl>

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
#include "captureselectdialog.h"

#include "zcore.h"

IManager* global_manager;
typedef void (__cdecl *ZCORE_GET_GLOBAL_MANAGER)(IManager**);

#define STAT_PERIOD 3

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    SettingsManager::setGlobalSettingsFilePath(QDir::homePath() + "/zgui.ini");

    ui->setupUi(this);

    /* XXX: на будущее
    QMenu * testMenu = new QMenu("For test", this);
    ui->menuBar->addMenu(testMenu);

    testMenu->addAction(tr("test HTML-render"), this, SLOT(onTestHtmlRender()));
    testMenu->addAction(tr("add screen capture"), this, SLOT(onAddScreenCapture()));
    */

    loadSplitterSettings();
    start();


    _audioPanel = new CAudioPanel(this);
    ui->bottom->addWidget(_audioPanel);


    //
    QToolBar* tbar = ui->mainToolBar;    
    tbar->setIconSize(QSize(64,64));


    // cam  (обработку отдаём в ScenePanel)
    connect(tbar->addAction(QIcon(":cam"), tr("Add camera")),
            &QAction::triggered, _scenePanel, &CScenePanel::onVideoCaptureSelect);
    /*
    tbar->addAction(QIcon(":cam2"), tr("Add camera"));
    tbar->addAction(QIcon(":cam2"), tr("Add camera"));
    tbar->addAction(QIcon(":cam2"), tr("Add camera"));
    tbar->addAction(QIcon(":cam2"), tr("Add camera"));
    tbar->addAction(QIcon(":cam2"), tr("Add camera"));
    */
    // images (обработку отдаём в ScenePanel)
    connect(tbar->addAction(QIcon(":img"), tr("Add image")),
            &QAction::triggered, _scenePanel, &CScenePanel::onImageSelect );


    // subscene
    connect(tbar->addAction(QIcon(":scene"), tr("Add sub-scene")),
            &QAction::triggered, this, &MainWindow::on_menusubscene_triggered);

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
}

MainWindow::~MainWindow()
{
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
       qDebug() << m_zcoreLib.errorString();
    }

    ZCORE_GET_GLOBAL_MANAGER ggm =(ZCORE_GET_GLOBAL_MANAGER)m_zcoreLib.resolve("getGlobalManager");
    ggm(&global_manager);

    // получаем размеры рабочей области
    SettingsManager setting("Settings");
    QString wsize = setting.getStringValue("Worksize");
    wsize = (wsize == ""?"640x360":wsize);
    QStringList sz = wsize.split("x");
    uint w = sz[0].toInt();
    uint h = sz[1].toInt();

    global_manager->startPipeline(w, h);

    _scenePanel = new CScenePanel(100, this);
    ui->top->addWidget(_scenePanel);

    //
    int ptr = setting.getIntValue("Workpattern");
    global_manager->setBackground(ptr);

    // таймер для air-статистики
    air_timer = new QTimer(this);
    connect(air_timer, SIGNAL(timeout()), this, SLOT(updateAirStat()));
}

void MainWindow::onAudioCaptureSelect()
{    
    CaptureSelectDialog dlg(CaptureSelectDialog::AudioDevice);
    if(dlg.exec() == QDialog::Accepted){
        QString src = dlg.getDevice();

        if(global_manager->addAudioSource(src.toLocal8Bit().data()))
        {
            CVolumeWidget *vw = new CVolumeWidget(src, this);

            _audioPanel->addVolumeWidget(vw);
        }
    }

}

void MainWindow::on_menusubscene_triggered()
{
    _scenePanel->addSubSceneLayer();
}


void MainWindow::onTestHtmlRender()
{
    QString fn = QFileDialog::getOpenFileName(this);
    _scenePanel->addHtmlRenderLayer(QUrl::fromLocalFile(fn).toString());
}

void MainWindow::onAddScreenCapture()
{
    RectSelectionWidget * w = new RectSelectionWidget();
    connect(w, SIGNAL(cancelled()),
            w, SLOT(deleteLater()));
    connect(w, SIGNAL(submitted()),
            this, SLOT(onScreenCaptureSelected()));
    w->show();
}

void MainWindow::onScreenCaptureSelected()
{
    RectSelectionWidget * w = qobject_cast<RectSelectionWidget*>(sender());
    if (!w) {
        qCritical() << "MainWindow::onScreenCaptureSelected(): sender should be RectSelectionWidget";
        return;
    }
    QRect rect = w->geometry();
    QString rectStr = QString("%1,%2,%3,%4")
            .arg(rect.x()).arg(rect.y())
            .arg(rect.width()).arg(rect.height());

    _scenePanel->addScreenCaptureLayer(rectStr);
    w->close();
    w->deleteLater();
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
            air_timer->start(STAT_PERIOD*1000);
            m_total_bytes = 0;
            m_total_frames = 0;
            m_air->setOnAir(dlg->test_mode);
            updateAirStat();            
            m_air_info->setDisabled(false);
        }
    }
    else
    {
        air_timer->stop();
        m_air->setStop();
        m_air_info->setText(" ");
        m_air_info->setDisabled(true);
        global_manager->stopAir();
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

}
