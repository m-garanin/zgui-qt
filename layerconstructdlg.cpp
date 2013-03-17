#include "layerconstructdlg.h"
#include "scenepanel.h"

#include <QMenuBar>
#include <QLayout>

CLayerConstructDlg::CLayerConstructDlg(QWidget *parent) :
    QDialog(parent)
{
    resize(800, 400);
    QMenuBar *menuBar = new QMenuBar;

    QMenu *camMenu = menuBar->addMenu(tr("Add Cam"));
    
    QAction *action;
    action = new QAction(tr("VS A"), this);
    connect(action, SIGNAL(triggered()), SLOT(onVSTriggered()));
    camMenu->addAction(action);

    action = new QAction(tr("VS B"), this);
    connect(action, SIGNAL(triggered()), SLOT(onVSTriggered()));
    camMenu->addAction(action);
    
    QMenu *imageMenu = menuBar->addMenu(tr("Add Image"));

    action = new QAction(tr("Image 1"), this);
    connect(action, SIGNAL(triggered()), SLOT(onImageTriggered()));
    imageMenu->addAction(action);

    action = new QAction(tr("Image 2"), this);
    connect(action, SIGNAL(triggered()), SLOT(onImageTriggered()));
    imageMenu->addAction(action);
    
    _scenePanel = new CScenePanel(100, this);
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

}
