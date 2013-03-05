#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "utils.cpp"
#include "IManager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createWidgets();
    fillVideoCaptureMenu();
    fillAudioCaptureMenu();

    //
    connect(this->ui->menuAdd_Cam, &QMenu::triggered, this, &MainWindow::on_menucam_triggered);
    connect(this->ui->menuAdd_Sound, &QMenu::triggered, this, &MainWindow::on_menusound_triggered);

    //this->ui->menuAdd_Image->ac
    QAction* act = this->ui->menuBar->addAction("Add Image");
    connect(act, &QAction::triggered, this, &MainWindow::on_menuimage_triggered);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createWidgets()
{

    prvScene = new PreviewWidget(this);

    for(int i=0; i<9; i++){
        vslot[i] = new QPushButton("Press me ", this);
    }


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


    for(int i=0; i<9; i++){
        if( i>0 && i % 3 == 0 ){
            sy += sh;
            sx = w/2;
        }
        qDebug() << "SLOT " << i << sx << sy;
        vslot[i]->setGeometry(sx, sy, sw, sh);
        vslot[i]->show();
        sx += sw;

    }

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    rePosition();
}

void MainWindow::on_startButton_clicked()
{

    //manager->startPipeline();
    qDebug() << "TEST A";
    init_core();
    qDebug() << "TEST B";
    int i = global_manager->test();
    qDebug() << "TEST:" << i;
    global_manager->startPipeline(640, 360);
    prvScene->start(100, 40);
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
    qDebug() << "ON MENU CAM TRIGGERED:" << act->text();
}

void MainWindow::on_menusound_triggered(QAction *act)
{
    qDebug() << "ON MENU SOUND TRIGGERED:" << act->text();
}

void MainWindow::on_menuimage_triggered()
{
    qDebug() << "ON MENU IMAGE TRIGGERED" ;
}


void MainWindow::fillVideoCaptureMenu()
{
    QStringList list = getVideoCaptureDevices();
    this->ui->menuAdd_Cam->clear();
    for (int i = 0; i < list.size(); i++){
        QAction* act = this->ui->menuAdd_Cam->addAction(list[i]);
    }

}

void MainWindow::fillAudioCaptureMenu()
{
    QStringList list = getAudioCaptureDevices();
    this->ui->menuAdd_Sound->clear();
    for (int i = 0; i < list.size(); i++){
        this->ui->menuAdd_Sound->addAction(list[i]);
    }
}
