#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QLayout>
#include <QLabel>
#include <QLibrary>

#include "airwidget.h"
#include "bigairstat.h"
#include "IManager.h"

namespace Ui {
class MainWindow;
}

class PreviewWidget;
class CScenePanel;
class CAudioPanel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:

    void on_menusubscene_triggered();

    void onAudioCaptureSelect();
    void onActionSettingsTriggered();
    void onAirTriggered();
    void onAirInfoTriggered();

    void updateAirStat();

    void onRecTriggered();
    void updateRecStat();

private:    
    void loadSplitterSettings();
    void saveSplitterSettings();

private:
    Ui::MainWindow *ui;
    QWidget* vslot[9];
    PreviewWidget* prvScene;        
    CScenePanel *_scenePanel;
    CAudioPanel *_audioPanel;
    QLibrary m_zcoreLib;

    // rec
    QToolButton* m_rec;
    QTimer *rec_timer; // таймер для статистики записи
    QToolButton* m_rec_info;


    // air
    AirWidget* m_air;
    QTimer *air_timer; // таймер для статистики трансляции
    uint64 m_total_bytes, m_total_frames;

    QToolButton* m_air_info;
    Bigairstat m_big_air_info;


    //
    void start();

signals:
    void recordStarting();
    void recordStoping();
    void airStarting();
    void airStoping();

};

#endif // MAINWINDOW_H
