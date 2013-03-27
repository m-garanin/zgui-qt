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

void StartAirDialog::loadValues()
{
    QStringList frameSizes, bitrates, instantWatchers;

    fillQuality();

    frameSizes << tr("320x240")
               << tr("640x480")
               << tr("1024x768")
               << tr("1280x1024");
    ui->frameSizeComboBox->addItems(frameSizes);

    bitrates << tr("64 kbps")
             << tr("256 kbps")
             << tr("512 kbps")
             << tr("1024 kbps");
    ui->bitrateComboBox->addItems(bitrates);

    instantWatchers << tr("10 watchers")
                    << tr("30 watchers")
                    << tr("70 watchers")
                    << tr("100 watchers")
                    << tr("Unlimited");
    ui->instantWatchersComboBox->addItems(instantWatchers);

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


