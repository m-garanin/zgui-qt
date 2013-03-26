#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QLayout>
#include <QLabel>

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

    void on_startRecordBtn_clicked(bool inProgress);
    void on_startAirBtn_clicked(bool inProgress);

    void updateMenuCornerWidget();
    void onTestHtmlRender();

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

    void start();

signals:
    void recordStarting();
    void recordStoping();
    void airStarting();
    void airStoping();

    void setRecordIndicatorText(QString text);
    void setAirIndicatorText(QString text);
};

#endif // MAINWINDOW_H
