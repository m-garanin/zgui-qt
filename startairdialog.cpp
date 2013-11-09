#include "startairdialog.h"
#include "ui_startairdialog.h"

#include "settingsmanager.h"
#include "IManager.h"

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
                << "800x600"
                << "1024x768"
                << "1280x960"                
                << "-- Widescreen: --"
                << "320x180"
                << "480x270"
                << "640x360"
                << "720x480"
                << "800x450"
                << "960x540"
                << "1024x576"
                << "1280x720"
                << "1366x768"
                << "1600x900"
                << "1920x1080";

    ui->frameSizeComboBox->addItems(frameSizes);

    bitrates << "64 Kbs"
             << "128 Kbs"
             << "256 Kbs"
             << "350 Kbs"
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
    startAir(0);
    done(QDialog::Accepted);
}

void StartAirDialog::on_testBtn_clicked()
{
    startAir(1);
    done(QDialog::Accepted);
}

void StartAirDialog::startAir(int test)
{
    QDir dir("QUALITYS");
    QString path = dir.absolutePath();
    QString param_fname, server_fname, log_fname;
    int w,h, br, acc;
    char tarif, quality;
    QStringList tmp;
    QString tmp_str;


    log_fname = QDir::homePath() + "/zgui_log.txt";

    param_fname = ui->encodingFormatComboBox->itemData( ui->encodingFormatComboBox->currentIndex()).toString() + ".par";
    param_fname = path + "/" + param_fname;

    // w x h
    tmp = ui->frameSizeComboBox->currentText().split("x");
    w = tmp[0].toInt();
    h = tmp[1].toInt();

    // bitrate
    tmp_str = ui->bitrateComboBox->currentText();
    if(tmp_str == "AUTO"){
        br = 0;
    }else{
        tmp = tmp_str.split(" ");
        br = tmp[0].toInt();
    }

    //
    tarif = ui->instantWatchersComboBox->itemData(ui->instantWatchersComboBox->currentIndex()).toString().toLocal8Bit().at(0);
    quality = ui->encodingFormatComboBox->itemData( ui->encodingFormatComboBox->currentIndex()).toString().toLocal8Bit().at(0);
    acc = ui->privateCheckBox->checkState() == Qt::Checked ;

    global_manager->startAir(ui->channelIdField->text().toInt(),
                             ui->passwordField->text().toLocal8Bit().data(),
                             param_fname.toLocal8Bit().data(),
                             server_fname.toLocal8Bit().data(), // TODO
                             log_fname.toLocal8Bit().data(),
                             w, h,
                             br,
                             tarif,
                             quality,
                             acc,
                             test);

    test_mode = test;
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



