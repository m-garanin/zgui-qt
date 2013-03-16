#ifndef LAYERCONSTRUCTDLG_H
#define LAYERCONSTRUCTDLG_H

#include <QDialog>

class CScenePanel;

class CLayerConstructDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit CLayerConstructDlg(QWidget *parent = 0);
    ~CLayerConstructDlg();

public slots:
    void onVSTriggered();
    void onImageTriggered();
    
private:
    CScenePanel *_scenePanel;
};

#endif // LAYERCONSTRUCTDLG_H
