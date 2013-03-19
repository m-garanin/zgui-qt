#include "startrecorddialog.h"
#include "ui_startrecorddialog.h"

#include "settingsmanager.h"

StartRecordDialog::StartRecordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartRecordDialog)
{
    ui->setupUi(this);

    fillLabels();
    loadValues();

    connect(this, SIGNAL(accepted()), this, SLOT(saveValues()));
}

StartRecordDialog::~StartRecordDialog()
{
    delete ui;
}

void StartRecordDialog::fillLabels()
{
    this->setWindowTitle(tr("Start Record"));
    ui->selectFolderLabel->setText(tr("Folder:"));
    ui->videoBitrateLabel->setText(tr("Video bitrate"));
    ui->audioBitrateLabel->setText(tr("Audio bitrate"));
    ui->kpsLabel1->setText(tr("kps"));
    ui->kpsLabel2->setText(tr("kps"));
}

void StartRecordDialog::loadValues()
{
    SettingsManager * values = new SettingsManager("RecordDialog");

    QString folderPath = values->getStringValue("folderPath");
    QString videoBitrate = values->getStringValue("videoBitrate");
    QString audioBitrate = values->getStringValue("audioBitrate");

    delete values;

    ui->folderNameField->setText(folderPath);
    ui->videoBitrateField->setText(videoBitrate);
    ui->audioBitrateField->setText(audioBitrate);
}

void StartRecordDialog::on_selectFolderBtn_clicked()
{
    ui->folderNameField->setText(QFileDialog::getExistingDirectory(this));
}

void StartRecordDialog::saveValues()
{
    SettingsManager * values = new SettingsManager("RecordDialog");

    values->setValue("folderPath", ui->folderNameField->text());
    values->setValue("videoBitrate", ui->videoBitrateField->text());
    values->setValue("audioBitrate", ui->audioBitrateField->text());

    delete values;
}
