#include "startairdialog.h"
#include "ui_startairdialog.h"

#include "settingsmanager.h"

#include <QDir>
#include <QTextStream>
#include <QDebug>

StartAirDialog::StartAirDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartAirDialog)
{
    ui->setupUi(this);

    fillLabels();
    loadValues();

    connect(this, SIGNAL(accepted()), this, SLOT(saveValues()));
    ui->passwordField->installEventFilter(this);
}

StartAirDialog::~StartAirDialog()
{
    delete ui;
}

bool StartAirDialog::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->passwordField)
    {
        if(event->type() == QEvent::FocusIn)
        {
            ui->passwordField->setEchoMode(QLineEdit::Normal);
        }
        if(event->type() == QEvent::FocusOut)
        {
            ui->passwordField->setEchoMode(QLineEdit::Password);
        }
    }
    return QDialog::eventFilter(obj, event);
}

void StartAirDialog::fillLabels()
{
    this->setWindowTitle(tr("Start Air"));
    ui->encodingParamsLabel->setText(tr("Select encoding params"));
    ui->encodingFormatLabel->setText(tr("Encoding format"));
    ui->frameSizeLabel->setText(tr("Frame size"));
    ui->bitrateLabel->setText(tr("Average Bitrate"));

    ui->cinemaRoomLabel->setText(tr("Select cinema room and its privacy"));
    ui->instantWatchersLabel->setText(tr("Instant watchers count"));
    ui->privateLabel->setText(tr("Limit access to the translation"));

    ui->startLabel->setText(tr("Now, start the translation or test the params"));
    ui->channelIdLabel->setText(tr("Channel ID"));
    ui->passwordLabel->setText(tr("Password"));
    ui->startBtn->setText(tr("Start"));
    ui->testBtn->setText(tr("Test"));


}

void StartAirDialog::loadValues()
{
    QStringList frameSizes, bitrates, instantWatchers;

    fillQuality();

    frameSizes  << "320x240"
                << "480x360"
                << "640x480"
                << "720x576"
                << "1024x768"
                << "1280x960"
                << "-- 16:9 --"
                << "320x180"
                << "480x270"
                << "640x360"
                << "720x480"
                << "1024x576"
                << "1280x720"
                << "1366x768"
                << "1600x900"
                << "1920x1080";

    ui->frameSizeComboBox->addItems(frameSizes);

    bitrates << "64 Kbs"
             << "128 Kbs"
             << "256 Kbs"
             << "512 Kbs"
             << "768 Kbs"
             << "1024 Kbs"
             << "AUTO";

    ui->bitrateComboBox->addItems(bitrates);


    instantWatchers << "A:Beginners(3 viewers)"
                    << "B:Home (10 viewers)"
                    << "C:Friends(50 viewers)"
                    << "D:Middle  (100 viewers)"
                    << "E:Big  (300 viewers)"
                    << "G:Unlim";

    QStringList tmp;
    for (int i = 0; i < instantWatchers.size(); ++i){
        tmp = instantWatchers.at(i).split(":");
        ui->instantWatchersComboBox->addItem(tmp[1], QVariant(tmp[0]));
    }


    //////////////////////
    SettingsManager * values = new SettingsManager("AirDialog");

    int encodingFormatValue = values->getIntValue("encodingFormat");
    int frameSizeValue = values->getIntValue("frameSize");
    int bitrateValue = values->getIntValue("bitrate");
    int instantWatchersValue = values->getIntValue("instantWatchers");
    bool isPrivate = values->getBoolValue("isPrivate");
    QString channelId = values->getStringValue("channelId");
    QString password = values->getStringValue("password");

    delete values;

    ui->encodingFormatComboBox->setCurrentIndex(encodingFormatValue);
    ui->frameSizeComboBox->setCurrentIndex(frameSizeValue);
    ui->bitrateComboBox->setCurrentIndex(bitrateValue);
    ui->instantWatchersComboBox->setCurrentIndex(instantWatchersValue);
    ui->privateCheckBox->setChecked(isPrivate);
    ui->channelIdField->setText(channelId);
    ui->passwordField->setText(password);
}

void StartAirDialog::on_startBtn_clicked()
{
    done(QDialog::Accepted);
}

void StartAirDialog::saveValues()
{
    SettingsManager * values = new SettingsManager("AirDialog");

    values->setValue("encodingFormat", ui->encodingFormatComboBox->currentIndex());
    values->setValue("frameSize", ui->frameSizeComboBox->currentIndex());
    values->setValue("bitrate", ui->bitrateComboBox->currentIndex());
    values->setValue("instantWatchers", ui->instantWatchersComboBox->currentIndex());
    values->setValue("isPrivate", ui->privateCheckBox->checkState());
    values->setValue("channelId", ui->channelIdField->text());
    values->setValue("password", ui->passwordField->text());

    delete values;
}


void StartAirDialog::fillQuality()
{
    QDir dir("QUALITYS");
    QString path = dir.absolutePath();


    QString fn = path + "/1.nbc";
    QFile f(fn);
    f.open(QFile::ReadOnly);

    QTextStream stream(&f);
    QString line;
    QStringList tmp;
    QString title;
    do {
        line = stream.readLine();
        if(line == "")
            break;

        tmp = line.split(":");
        title = tmp[1];
        ui->encodingFormatComboBox->addItem(title, QVariant(tmp[0]) );
    } while (!line.isNull());

}


