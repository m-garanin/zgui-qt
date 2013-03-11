#include "startrecorddialog.h"
#include "ui_startrecorddialog.h"

StartRecordDialog::StartRecordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartRecordDialog)
{
    ui->setupUi(this);

    fillLabels();
}

StartRecordDialog::~StartRecordDialog()
{
    delete ui;
}

void StartRecordDialog::fillLabels()
{
    ui->selectFileLabel->setText(tr("Filename:"));
    ui->videoBitrateLabel->setText(tr("Video bitrate"));
    ui->audioBitrateLabel->setText(tr("Audio bitrate"));
    ui->kpsLabel1->setText(tr("kps"));
    ui->kpsLabel2->setText(tr("kps"));
}
