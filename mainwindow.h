#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QLayout>
#include <QLabel>
#include <QLibrary>

#include "menubarwidget.h"

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

    void on_menucam_triggered(QAction* act);
    void on_menusound_triggered(QAction* act);
    void on_menuimage_triggered();
    void on_menusubscene_triggered();

    void updateMenuCornerWidget();
    void onTestHtmlRender();
    void onAddScreenCapture();
    void onScreenCaptureSelected();
    void onActionSettingsTriggered();

private:
    void fillVideoCaptureMenu();
    void fillAudioCaptureMenu();

    void loadSplitterSettings();
    void saveSplitterSettings();

private:
    Ui::MainWindow *ui;
    QWidget* vslot[9];
    PreviewWidget* prvScene;
    QString pathToSettings;
    MenuBarWidget * menuBarWidget;
    CScenePanel *_scenePanel;
    CAudioPanel *_audioPanel;
    QLibrary m_zcoreLib;

    void start();

signals:
    void recordStarting();
    void recordStoping();
    void airStarting();
    void airStoping();

};

#endif // MAINWINDOW_H
