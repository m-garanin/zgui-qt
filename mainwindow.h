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

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    QString selectEffects(quint32 cols);

private slots:

    void on_menucam_triggered(QAction* act);
    void on_menusound_triggered(QAction* act);
    void on_menuimage_triggered();
    void on_menusubscene_triggered();

    void on_startRecordBtn_clicked(bool inProgress);
    void on_startAirBtn_clicked(bool inProgress);

private:
    void fillVideoCaptureMenu();
    void fillAudioCaptureMenu();

private:
    Ui::MainWindow *ui;
    QWidget* vslot[9];
    PreviewWidget* prvScene;
    QString pathToSettings;    
    MenuBarWidget * menuBarWidget;
    CScenePanel *_scenePanel;

    void start();

signals:
    void recordStarting();
    void recordStoping();
    void airStarting();
    void airStoping();
};

#endif // MAINWINDOW_H
