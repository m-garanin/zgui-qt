#include <QDateTime>
#include <QDir>
#include "startrecorddialog.h"
#include "ui_startrecorddialog.h"

#include "settingsmanager.h"
#include "utils.h"
#include "IManager.h"

StartRecordDialog::StartRecordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartRecordDialog)
{
    ui->setupUi(this);

    fillLabels();
    loadValues();

    connect(this, SIGNAL(accepted()), this, SLOT(accepted()));
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
    ui->kpsLabel1->setText(tr("Kbs"));
    ui->kpsLabel2->setText(tr("Kbs"));
}

void StartRecordDialog::loadValues()
{
    SettingsManager * values = new SettingsManager("RecordDialog");

    QString folderPath = values->getStringValue("folderPath");
    QString videoBitrate = values->getStringValue("videoBitrate");
    QString audioBitrate = values->getStringValue("audioBitrate");

    delete values;

    ui->folderNameField->setText(folderPath==""?QDir::homePath():folderPath);
    ui->videoBitrateField->setText(videoBitrate==""?"2000":videoBitrate);
    ui->audioBitrateField->setText(audioBitrate==""?"128":audioBitrate);
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

void StartRecordDialog::accepted()
{
    saveValues();
    QDateTime dt =  QDateTime::currentDateTime();
    QString fname = ui->folderNameField->text() + "/" + dt.toString("dd.MM.yy-hh.mm") + ".ts";

    // получаем размеры рабочей области (TODO XXX: дублирование кода)    
    QString wsize = getWorksize();
    QStringList sz = wsize.split("x");
    uint w = sz[0].toInt();
    uint h = sz[1].toInt();


    global_manager->startRec(fname.toLocal8Bit().data(), w, h,
                             ui->videoBitrateField->text().toInt(),
                             ui->audioBitrateField->text().toInt());
}

