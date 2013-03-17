#include "startairdialog.h"
#include "ui_startairdialog.h"

StartAirDialog::StartAirDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartAirDialog)
{
    ui->setupUi(this);

    fillLabels();
}

StartAirDialog::~StartAirDialog()
{
    delete ui;
}

void StartAirDialog::fillLabels()
{
    this->setWindowTitle(tr("Start Air"));
    ui->encodingParamsLabel->setText(tr("Select encoding params"));
    ui->encodingFormatLabel->setText(tr("Encoding format"));
    ui->frameSizeLabel->setText(tr("Frame size"));
    ui->bitrateLabel->setText(tr("Bitrate"));

    ui->cinemaRoomLabel->setText(tr("Select cinema room and its privacy"));
    ui->instantWatchersLabel->setText(tr("Instant watchers count"));
    ui->privateLabel->setText(tr("Limit access to the translation"));

    ui->startLabel->setText(tr("Now, start the translation or test the params"));
    ui->channelIdLabel->setText(tr("Channel ID"));
    ui->passwordLabel->setText(tr("Password"));
    ui->startBtn->setText(tr("Start"));
    ui->testBtn->setText(tr("Test"));
}

void StartAirDialog::on_startBtn_clicked()
{
    done(QDialog::Accepted);
}
