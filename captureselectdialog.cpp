#include "captureselectdialog.h"
#include "ui_captureselectdialog.h"

#include "utils.cpp"
#include <QPushButton>

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
    m_device = btn->text();
    this->done(QDialog::Accepted);
}

void CaptureSelectDialog::fillDevices(CaptureDeviceType type)
{
    QStringList list;
    if(type == VideoDevice){
        list = getVideoCaptureDevices();
    }else{
        list = getAudioCaptureDevices();
    }

    for (int i = 0; i < list.size(); i++){
        QPushButton* btn = new QPushButton(list[i]);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        ui->verticalLayout->addWidget(btn);

        connect(btn, SIGNAL(clicked()), SLOT(onSelect()));
    }

}

