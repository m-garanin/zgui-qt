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

protected:
    void showEvent(QShowEvent * event);

public slots:
    void onVSTriggered();
    void onImageTriggered();

    void onAccepted();
    void onRejected();
    
private:
    CScenePanel *_scenePanel;
};

#endif // LAYERCONSTRUCTDLG_H
