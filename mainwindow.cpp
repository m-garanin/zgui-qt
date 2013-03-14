#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "previewwidget.h"

#include "volumewidget.h"
#include "audiopanel.h"
#include "scenepanel.h"

#ifdef Q_OS_WIN32
#include "utils.cpp"
#include "IManager.h"
#endif

#include "effectsdlg.h"
#include "startairdialog.h"
#include "startrecorddialog.h"

#include <QDebug>
#include <QFileDialog>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pathToSettings("settings.ini")
{
    ui->setupUi(this);

    fillVideoCaptureMenu();
    fillAudioCaptureMenu();

    connect(this->ui->menuAdd_Cam, &QMenu::triggered, this, &MainWindow::on_menucam_triggered);
    connect(this->ui->menuAdd_Sound, &QMenu::triggered, this, &MainWindow::on_menusound_triggered);


    QAction* act = this->ui->menuBar->addAction("Add Image");
    connect(act, &QAction::triggered, this, &MainWindow::on_menuimage_triggered);

    QAction* act2 = this->ui->menuBar->addAction("Add Sub-Scene");
    connect(act2, &QAction::triggered, this, &MainWindow::on_menusubscene_triggered);


    menuBarWidget = new MenuBarWidget(ui->menuBar);
    menuBarWidget->setMaximumSize(menuBarWidget->width(), menuBarWidget->height());
    ui->menuBar->setCornerWidget(menuBarWidget, Qt::TopRightCorner);

    connect(menuBarWidget, SIGNAL(startAirBtnClicked(bool)), SLOT(on_startAirBtn_clicked(bool)));
    connect(menuBarWidget, SIGNAL(startRecordBtnClicked(bool)), SLOT(on_startRecordBtn_clicked(bool)));
    connect(this, SIGNAL(recordStarting()), menuBarWidget, SLOT(recordStarting()));
    connect(this, SIGNAL(recordStoping()), menuBarWidget, SLOT(recordStoping()));
    connect(this, SIGNAL(airStarting()), menuBarWidget, SLOT(airStarting()));
    connect(this, SIGNAL(airStoping()), menuBarWidget, SLOT(airStoping()));

    start();
}

MainWindow::~MainWindow()
{
    delete menuBarWidget;
    delete ui;
}

void MainWindow::start()
{
    init_core();
    global_manager->startPipeline(640, 360);
    _scenePanel = new CScenePanel(100, this);
    ui->verticalLayout_2->addWidget(_scenePanel);
}

QString MainWindow::selectEffects(quint32 cols)
{
    CEffectsDlg eff;
    eff.setColuntCount(cols);
    eff.exec();
    return eff.selectedEffectName();
}



void MainWindow::on_menucam_triggered(QAction *act)
{
    _scenePanel->addCamLayer(act->text());
}

void MainWindow::on_menusound_triggered(QAction *act)
{
    /*
    qDebug() << "ON MENU SOUND TRIGGERED:" << act->text();

    CVolumeWidget *vw = new CVolumeWidget(50, this);
    vw->setText(act->text());

    ui->verticalLayout->addWidget(vw);
    */
}

void MainWindow::on_menuimage_triggered()
{
    QSettings settings(pathToSettings, QSettings::IniFormat);
    QString file = QFileDialog::getOpenFileName(this, "Add Image", settings.value("default_dir").toString(), "Image Files (*.png *.jpg *.bmp)");    
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
#ifdef Q_OS_WIN32
    QStringList list = getVideoCaptureDevices();
    this->ui->menuAdd_Cam->clear();
    for (int i = 0; i < list.size(); i++){
        QAction* act = this->ui->menuAdd_Cam->addAction(list[i]);
    }
#endif
}

void MainWindow::fillAudioCaptureMenu()
{
#ifdef Q_OS_WIN32
    QStringList list = getAudioCaptureDevices();
    this->ui->menuAdd_Sound->clear();
    for (int i = 0; i < list.size(); i++){
        this->ui->menuAdd_Sound->addAction(list[i]);
    }
#endif
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
        }
    }
    else
    {
        qDebug() << "stoping record";

        emit recordStoping();
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
            qDebug() << "starting air";

            emit airStarting();
        }
    }
    else
    {
        qDebug() << "stoping air";

        emit airStoping();
    }
}
