#include "layerconstructdlg.h"
#include "scenepanel.h"

#include "utils.h"

#include <QToolBar>
#include <QLayout>
#include <QDebug>
#include <QSettings>
#include <QDir>
#include <QFileDialog>
#include <QIcon>
#include <QAction>


CLayerConstructDlg::CLayerConstructDlg(qint32 compkey, QWidget *parent) :
    QDialog(parent, Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
    pathToSettings("settings.ini")
{
    resize(800, 400);
    setWindowTitle(tr("Sub Scene"));

    connect(this, SIGNAL(accepted()), SLOT(onAccepted()));
    connect(this, SIGNAL(rejected()), SLOT(onRejected()));

    _scenePanel = new CScenePanel(compkey, this);

    QToolBar *tbar = new QToolBar(this);
    tbar->setIconSize(QSize(64,64));
    tbar->setOrientation(Qt::Vertical);

    // cam  (обработку отдаём в ScenePanel)
    connect(tbar->addAction(QIcon(":cam"), tr("Add camera")),
            &QAction::triggered, _scenePanel, &CScenePanel::onVideoCaptureSelect);

    // images (обработку отдаём в ScenePanel)
    connect(tbar->addAction(QIcon(":img"), tr("Add image")),
            &QAction::triggered, _scenePanel, &CScenePanel::onImageSelect );


    // screen-capture (обработку отдаём в ScenePanel)
    connect(tbar->addAction(QIcon(":screen_capture"), tr("Add screen capture")),
            &QAction::triggered, _scenePanel, &CScenePanel::onAddScreenCapture );

    ////

    QHBoxLayout *layout = new QHBoxLayout(this);
    //


    layout->addWidget(_scenePanel);
    layout->addWidget(tbar);
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















