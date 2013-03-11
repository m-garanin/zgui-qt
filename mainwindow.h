#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QLayout>
#include <QLabel>


namespace Ui {
class MainWindow;
}

class PreviewWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    QString selectEffects(quint32 cols);

private slots:

    void on_startButton_clicked();

    void on_pushButton_clicked();

    void on_menucam_triggered(QAction* act);
    void on_menusound_triggered(QAction* act);
    void on_menuimage_triggered();

    void onPbSelectEffectsClicked();
    void onPbPreviewWidgetClicked();
    void onPbApplyClicked();

    void on_testPreviewButton_clicked();

private:
    void fillVideoCaptureMenu();
    void fillAudioCaptureMenu();
    void createWidgets();
    void rePosition();

    void resizeEvent(QResizeEvent * event);

private:
    Ui::MainWindow *ui;
    QWidget* vslot[9];
    PreviewWidget* prvScene;
    QString pathToSettings;
    QLabel * imageLable;
};

#endif // MAINWINDOW_H
