#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QLayout>
#include <QLabel>
#include <QLibrary>
#include <QFile>
#include <QTimer>
#include <QMutex>

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
    
    CAudioPanel* getAudioPanel(){return _audioPanel;}

    void logger(char* buf);

private slots:

    void on_menusubscene_triggered();

    void onAudioCaptureSelect();
    void onActionSettingsTriggered();
    void onAirTriggered();
    void onAirInfoTriggered();

    void updateAirStat();

    void onRecTriggered();
    void updateRecStat();

    void statusBarTimerTimeout();
private:    
    void loadSplitterSettings();
    void saveSplitterSettings();

    void saveLastConfig();
    void restoreLastConfig();

    void setStatusBarMessage(QString msg);
private:
    Ui::MainWindow *ui;
    QFile* m_logfile;
    QWidget* vslot[9];
    PreviewWidget* prvScene;        
    CScenePanel *_scenePanel;
    CAudioPanel *_audioPanel;
    QLibrary m_zcoreLib;

    // status-bar loggger (для ui-вывода сообщений)
    // сообщения могут приходить из разных потоков, поэтому
    // используем работу через таймер
    QString m_statusbar_msg;
    QMutex  m_statusbar_mutex;
    QTimer* m_statusbar_timer;


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
    void log_fixstart();
    void detectVersion();

signals:
    void recordStarting();
    void recordStoping();
    void airStarting();
    void airStoping();

};

#endif // MAINWINDOW_H
