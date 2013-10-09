#include "captureselectdialog.h"
#include "ui_captureselectdialog.h"

#include "utils.cpp"
#include <QPushButton>
#include <QStringList>


CaptureSelectDialog::CaptureSelectDialog(CaptureDeviceType type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CaptureSelectDialog)
{
    ui->setupUi(this);
    fillDevices(type);
    if(type == VideoDevice){
        setWindowTitle(tr("Select video capture device"));
    }else{
        setWindowTitle(tr("Select audio capture device"));
    }
}

CaptureSelectDialog::~CaptureSelectDialog()
{
    delete ui;
}

void CaptureSelectDialog::onSelect()
{
    QPushButton* btn = (QPushButton*)sender();
    m_device = btn->property("DEVICE").toString();
    m_psize = ui->cbxPSize->currentText();

    this->done(QDialog::Accepted);
}

void CaptureSelectDialog::fillDevices(CaptureDeviceType type)
{
    QStringList list;
    if(type == VideoDevice){
        list = getVideoCaptureDevices();
        fillPSize();
        // TODO: возможно сделать только в `расширенном` интерфейсе
        ui->txtPSize->show();
        ui->cbxPSize->show();
    }else{
        ui->txtPSize->hide();
        ui->cbxPSize->hide();
        list = getAudioCaptureDevices();
    }

    for (int i = 0; i < list.size(); i++){
        QPushButton* btn = new QPushButton( friendlyDeviceName(list[i])  );
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        ui->verticalLayout->addWidget(btn);
        btn->setProperty("DEVICE", list[i]);
        connect(btn, SIGNAL(clicked()), SLOT(onSelect()));
    }

}

void CaptureSelectDialog::fillPSize()
{
    QStringList sizes;

    sizes  << "AUTO"
           << "320x240"
           << "480x360"
           << "640x480"
           << "720x576"
           << "800x600"
           << "1024x768"
           << "1280x960"
           << "----- Widescreen: -----"
           << "320x180"
           << "480x270"
           << "640x360"
           << "720x480"
           << "800x450"
           << "1024x576"
           << "1280x720"
           << "1366x768"
           << "1600x900"
           << "1920x1080";

    ui->cbxPSize->addItems(sizes);
}

