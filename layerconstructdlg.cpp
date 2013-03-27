#include "layerconstructdlg.h"
#include "scenepanel.h"

#include <QMenuBar>
#include <QLayout>
#include <QDebug>
#include <QSettings>
#include <QDir>
#include <QFileDialog>

CLayerConstructDlg::CLayerConstructDlg(qint32 compkey, QWidget *parent) :
    QDialog(parent, Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
    pathToSettings("settings.ini")
{
    resize(800, 400);
    setWindowTitle(tr("Sub Scene # %1").arg(compkey));

    connect(this, SIGNAL(accepted()), SLOT(onAccepted()));
    connect(this, SIGNAL(rejected()), SLOT(onRejected()));

    QMenuBar *menuBar = new QMenuBar(this);
    menuBar->setDefaultUp(false);
    menuBar->setNativeMenuBar(false);

    QMenu *camMenu = menuBar->addMenu(tr("Add Cam"));
    
    QAction *action;
    action = new QAction(tr("VS A"), this);
    connect(action, SIGNAL(triggered()), SLOT(onVSTriggered()));
    camMenu->addAction(action);

    action = new QAction(tr("VS B"), this);
    connect(action, SIGNAL(triggered()), SLOT(onVSTriggered()));
    camMenu->addAction(action);
    
    action = menuBar->addAction(tr("Add Image"));
    connect(action, SIGNAL(triggered()), SLOT(onImageTriggered()));
    
    _scenePanel = new CScenePanel(compkey, this);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMenuBar(menuBar);
    layout->addWidget(_scenePanel);
}

CLayerConstructDlg::~CLayerConstructDlg()
{
}

void CLayerConstructDlg::onVSTriggered()
{
    if(QAction *action = qobject_cast<QAction*>(sender()))
    {
        _scenePanel->addCamLayer(action->text());
    }
}

void CLayerConstructDlg::onImageTriggered()
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

void CLayerConstructDlg::onAccepted()
{
    _scenePanel->stop();
}

void CLayerConstructDlg::onRejected()
{
    _scenePanel->stop();
}

void CLayerConstructDlg::showEvent(QShowEvent *event)
{
    _scenePanel->start();
    QDialog::showEvent(event);
}















