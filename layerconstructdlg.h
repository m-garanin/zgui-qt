#ifndef LAYERCONSTRUCTDLG_H
#define LAYERCONSTRUCTDLG_H

#include <QDialog>

class CScenePanel;

class CLayerConstructDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit CLayerConstructDlg(qint32 compkey, QWidget *parent = 0);
    ~CLayerConstructDlg();

    CScenePanel* getScenePanel(){ return _scenePanel;}

protected:
    void showEvent(QShowEvent * event);

public slots:

    void onImageTriggered();

    void onAccepted();
    void onRejected();
    
private:
    CScenePanel *_scenePanel;
    QString pathToSettings;
};

#endif // LAYERCONSTRUCTDLG_H
