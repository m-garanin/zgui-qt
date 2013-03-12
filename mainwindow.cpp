#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "previewwidget.h"
#include "previewwidgettester.h"
#include "layerwidget.h"
#include "volumewidget.h"
#include "audiopanel.h"

#ifdef Q_OS_WIN32
#include "utils.cpp"
#include "IManager.h"
#endif

#include "effectsdlg.h"

#include <QDebug>
#include <QFileDialog>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pathToSettings("settings.ini")
{
    ui->setupUi(this);
    createWidgets();
    fillVideoCaptureMenu();
    fillAudioCaptureMenu();

    imageLable = new QLabel(this);
    ui->horizontalLayout->addWidget(imageLable);
    //
    connect(this->ui->menuAdd_Cam, &QMenu::triggered, this, &MainWindow::on_menucam_triggered);
    connect(this->ui->menuAdd_Sound, &QMenu::triggered, this, &MainWindow::on_menusound_triggered);

    //this->ui->menuAdd_Image->ac
    QAction* act = this->ui->menuBar->addAction("Add Image");
    connect(act, &QAction::triggered, this, &MainWindow::on_menuimage_triggered);

    connect(ui->pbSelectEffects, SIGNAL(clicked()), SLOT(onPbSelectEffectsClicked()));
    connect(ui->pbAddPreviewWidget, SIGNAL(clicked()), SLOT(onPbPreviewWidgetClicked()));
    connect(ui->pbApply, SIGNAL(clicked()), SLOT(onPbApplyClicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::selectEffects(quint32 cols)
{
    CEffectsDlg eff;
    eff.setColuntCount(cols);
    eff.exec();
    return eff.selectedEffectName();
}

void MainWindow::onPbSelectEffectsClicked()
{
    qDebug() << selectEffects(3);
}

void MainWindow::onPbPreviewWidgetClicked()
{
    ui->sceneWidget->showBox(1);
}

void MainWindow::onPbApplyClicked()
{
    qDebug() << ui->sceneWidget->apply();
}

void MainWindow::createWidgets()
{

    prvScene = new PreviewWidget(this);

    //for(int i=0; i<9; i++){
    //    vslot[i] = new QPushButton("Press me ", this);
    //}


}

void MainWindow::rePosition()
{
    int w,h, sw, sh, sx, sy;
    w = this->width();
    h = this->height();

    sw = w/(2*3);
    sh = 3*sw/4;

    sx = w/2;
    sy = this->ui->menuBar->height();

    prvScene->setGeometry(0, sy, w/2, 3*w/8);

    qDebug() << "SLOT SIZE:" << sw << "x"  << sh ;


    for(int i=0; i<listLayerWidgets.size(); i++){
        if( i>0 && i % 3 == 0 ){
            sy += sh;
            sx = w/2;
        }
        qDebug() << "SLOT " << i << sx << sy;
        listLayerWidgets[i]->setGeometry(sx, sy, sw, sh);
        listLayerWidgets[i]->show();
        sx += sw;

    }

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    rePosition();
}

void MainWindow::on_startButton_clicked()
{
//    init_core();
//    global_manager->startPipeline(640, 360);
//    int scene_id = global_manager->addScene();
//    prvScene->start(scene_id, 40);

}


void MainWindow::on_pushButton_clicked()
{
    //QWidget *button = new QPushButton("Press me", this);
    //button->setGeometry(50,100, 200,200);
    //button->show();
    rePosition();
}

void MainWindow::on_menucam_triggered(QAction *act)
{
    if (listLayerWidgets.count() >= 6)
    {
        qDebug() << "Cam max 6 element.";
        return;
    }
    qDebug() << "ON MENU CAM TRIGGERED:" << act->text();
    
    CLayerWidget *lw = new CLayerWidget(100, this);
    listLayerWidgets.push_back(lw);

    qint32 column_count = 3;
    qint32 count = listLayerWidgets.count() - 1;

    int row = count/column_count;
    int col = count%column_count;

    ui->tableWidget->setIndexWidget(ui->tableWidget->model()->index(row, col), listLayerWidgets.last());

    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    //rePosition();
}

void MainWindow::on_menusound_triggered(QAction *act)
{
    qDebug() << "ON MENU SOUND TRIGGERED:" << act->text();

    CVolumeWidget *vw = new CVolumeWidget(50, this);
    vw->setText(act->text());

    ui->verticalLayout->addWidget(vw);
}

void MainWindow::on_menuimage_triggered()
{
    QSettings settings(pathToSettings, QSettings::IniFormat);
    QString file = QFileDialog::getOpenFileName(this, "Add Image", settings.value("default_dir").toString(), "Image Files (*.png *.jpg *.bmp)");    
    if (!file.isEmpty()) 
    { 
        QDir curDir(file);
        settings.setValue("default_dir", curDir.absolutePath());        
        QImage myImage;
        myImage.load(file);
        imageLable->setPixmap(QPixmap::fromImage(myImage));
    }
}


void MainWindow::fillVideoCaptureMenu()
{
#ifdef Q_OS_WIN32
    QStringList list = getVideoCaptureDevices();
    this->ui->menuAdd_Cam->clear();
    for (int i = 0; i < list.size(); i++){
        QAction* act = this->ui->menuAdd_Cam->addAction(list[i]);
    }
#endif
}

void MainWindow::fillAudioCaptureMenu()
{
#ifdef Q_OS_WIN32
    QStringList list = getAudioCaptureDevices();
    this->ui->menuAdd_Sound->clear();
    for (int i = 0; i < list.size(); i++){
        this->ui->menuAdd_Sound->addAction(list[i]);
    }
#endif
}

void MainWindow::on_testPreviewButton_clicked()
{
    QWidget * w = new PreviewWidgetTester();
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}
