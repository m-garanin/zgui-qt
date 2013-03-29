#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "volumewidget.h"
#include "audiopanel.h"
#include "scenepanel.h"

#include "utils.cpp"
#include "IManager.h"

#include "startairdialog.h"
#include "startrecorddialog.h"

#include "settingsdlg.h"

#include "rectselectionwidget.h"

#include <QDebug>
#include <QFileDialog>
#include <QStringList>
#include <QScrollArea>
#include <QPushButton>
#include <QTimer>
#include <QTextStream>

#include <QUrl>

#include "settingsmanager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pathToSettings("settings.ini")
{
    SettingsManager::setGlobalSettingsFilePath(pathToSettings);

    ui->setupUi(this);

    fillVideoCaptureMenu();
    fillAudioCaptureMenu();

    connect(this->ui->menuAdd_Cam, &QMenu::triggered, this, &MainWindow::on_menucam_triggered);
    connect(this->ui->menuAdd_Sound, &QMenu::triggered, this, &MainWindow::on_menusound_triggered);


    QAction* act = this->ui->menuBar->addAction("Add Image");
    connect(act, &QAction::triggered, this, &MainWindow::on_menuimage_triggered);

    QAction* act2 = this->ui->menuBar->addAction("Add Sub-Scene");
    connect(act2, &QAction::triggered, this, &MainWindow::on_menusubscene_triggered);

    QMenu * testMenu = new QMenu("For test", this);
    ui->menuBar->addMenu(testMenu);

    testMenu->addAction(tr("test HTML-render"), this, SLOT(onTestHtmlRender()));
    testMenu->addAction(tr("add screen capture"), this, SLOT(onAddScreenCapture()));

    QAction* settings = this->ui->menuBar->addAction("Settings");
    connect(settings, SIGNAL(triggered()), SLOT(onActionSettingsTriggered()));



    menuBarWidget = new MenuBarWidget(ui->menuBar);
    menuBarWidget->setMaximumSize(menuBarWidget->width(), menuBarWidget->height());
    this->updateMenuCornerWidget();

    connect(menuBarWidget, SIGNAL(startAirBtnClicked(bool)), SLOT(on_startAirBtn_clicked(bool)));
    connect(menuBarWidget, SIGNAL(startRecordBtnClicked(bool)), SLOT(on_startRecordBtn_clicked(bool)));
    connect(this, SIGNAL(recordStarting()), menuBarWidget, SLOT(recordStarting()));
    connect(this, SIGNAL(recordStoping()), menuBarWidget, SLOT(recordStoping()));
    connect(this, SIGNAL(airStarting()), menuBarWidget, SLOT(airStarting()));
    connect(this, SIGNAL(airStoping()), menuBarWidget, SLOT(airStoping()));
    connect(this, SIGNAL(setRecordIndicatorText(QString)), menuBarWidget, SIGNAL(setRecordIndicatorText(QString)));
    connect(this, SIGNAL(setAirIndicatorText(QString)), menuBarWidget, SIGNAL(setAirIndicatorText(QString)));
    connect(this, SIGNAL(setRecordIndicatorText(QString)), this, SLOT(updateMenuCornerWidget()));
    connect(this, SIGNAL(setAirIndicatorText(QString)), this, SLOT(updateMenuCornerWidget()));

    loadSplitterSettings();
    start();

    _audioPanel = new CAudioPanel;
    ui->scrollArea->setWidget(_audioPanel);
}

MainWindow::~MainWindow()
{
    saveSplitterSettings();
    delete menuBarWidget;
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
    init_core();
    global_manager->startPipeline(640, 360); // TODO: 640? maybe 480?
    _scenePanel = new CScenePanel(100, this);
    _scenePanel->show();
    ui->verticalLayout_2->addWidget(_scenePanel);
}

void MainWindow::on_menucam_triggered(QAction *act)
{
    _scenePanel->addCamLayer(act->text());
}

void MainWindow::on_menusound_triggered(QAction *act)
{
    qDebug() << "ON MENU SOUND TRIGGERED:" << act->text();

    if(global_manager->addAudioSource(act->text().toUtf8().data()))
    {
        CVolumeWidget *vw = new CVolumeWidget(act->text(), 0.1, this);
        vw->setText(act->text());

        _audioPanel->addVolumeWidget(vw);
    }
}

void MainWindow::on_menuimage_triggered()
{
    SettingsManager settings("MainWindow");
    QString file = QFileDialog::getOpenFileName(this, "Add Image", settings.getStringValue("default_dir"), "Image Files (*.png *.jpg *.bmp)");
    if (!file.isEmpty()) 
    { 
        QDir curDir(file);
        settings.setValue("default_dir", curDir.absolutePath());        
        _scenePanel->addImageLayer(file);
    }
}

void MainWindow::on_menusubscene_triggered()
{
    _scenePanel->addSubSceneLayer();
}


void MainWindow::fillVideoCaptureMenu()
{
    QStringList list;
    list = getVideoCaptureDevices();
    this->ui->menuAdd_Cam->clear();
    for (int i = 0; i < list.size(); i++){
        QAction* act = this->ui->menuAdd_Cam->addAction(list[i]);
    }
}

void MainWindow::fillAudioCaptureMenu()
{
    QStringList list;
    list = getAudioCaptureDevices();

    this->ui->menuAdd_Sound->clear();
    for (int i = 0; i < list.size(); i++){
        this->ui->menuAdd_Sound->addAction(list[i]);
    }
}


void MainWindow::on_startRecordBtn_clicked(bool inProgress)
{
    if(!inProgress)
    {
        StartRecordDialog * startRecordDialog = new StartRecordDialog(this);
        startRecordDialog->setAttribute(Qt::WA_DeleteOnClose);

        if(startRecordDialog->exec() == QDialog::Accepted)
        {
            qDebug() << "starting record";

            emit recordStarting();

            emit setRecordIndicatorText("1020 MB");
        }
    }
    else
    {
        qDebug() << "stoping record";

        emit recordStoping();

        emit setRecordIndicatorText("Start Record");
    }
}

void MainWindow::on_startAirBtn_clicked(bool inProgress)
{
    if(!inProgress)
    {
        StartAirDialog * startAirDialog = new StartAirDialog(this);
        startAirDialog->setAttribute(Qt::WA_DeleteOnClose);

        if(startAirDialog->exec() == QDialog::Accepted)
        {
            emit airStarting();
            emit setAirIndicatorText("On Air 25fps, 512 Kbs");
        }
    }
    else
    {        
        global_manager->stopAir();

        emit airStoping();
        emit setAirIndicatorText("Start Air");
    }
}

void MainWindow::updateMenuCornerWidget()
{
    ui->menuBar->setCornerWidget(menuBarWidget, Qt::TopRightCorner);
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
    if(dlg.exec() == QDialog::Accepted)
    {
        _scenePanel->applySetting();
    }
}
