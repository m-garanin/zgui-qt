#include "settingsdlg.h"
#include "ui_settingsdlg.h"
#include "settingsmanager.h"
#include "IManager.h"


#include <QMessageBox>
#include <QDebug>

CSettingsDlg::CSettingsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSettingsDlg)
{
    ui->setupUi(this);

    //qDebug() << "QGLFormat::hasOpenGLOverlays() = " << QGLFormat::hasOpenGLOverlays();
    //qDebug() << "QGLFormat::openGLVersionFlags() = " << QGLFormat::openGLVersionFlags();

    connect(ui->pbApply, SIGNAL(clicked()), SLOT(onPbApplyClicked()));
    connect(ui->pbClose, SIGNAL(clicked()), SLOT(reject()));
    SettingsManager setting("Settings");

    /*
    if(QGLFormat::hasOpenGL())
        ui->cbOpenGL->setChecked(setting.getBoolValue("OpenGL"));
    else
        ui->cbOpenGL->setEnabled(false);
    */
    fillWorksizes(setting.getStringValue("Worksize"));
}

CSettingsDlg::~CSettingsDlg()
{
    delete ui;
}

void CSettingsDlg::onPbApplyClicked()
{
    SettingsManager setting("Settings");
    QString wsize = ui->workspaceSizeComboBox->currentText();
    //setting.setValue("OpenGL", ui->cbOpenGL->isChecked());

    if(wsize != setting.getStringValue("Worksize")){
        setting.setValue("Worksize", wsize );
        QStringList sz = wsize.split("x");
        uint w = sz[0].toInt();
        uint h = sz[1].toInt();
        //qDebug() << "WORKSIZE CHANGE" << w << "x" << h;
        global_manager->setWorksize(w, h);
    }

    accept();
}

void CSettingsDlg::fillWorksizes(QString val)
{
    QStringList sizes;
    sizes  << "320x240"
           << "480x360"
           << "640x480"
           << "720x576"
           << "1024x768"
           << "1280x960"
           << "----- Widescreen: -----"
           << "320x180"
           << "480x270"
           << "640x360"
           << "720x480"
           << "1024x576"
           << "1280x720"
           << "1366x768"
           << "1600x900"
           << "1920x1080";

    ui->workspaceSizeComboBox->addItems(sizes);
    ui->workspaceSizeComboBox->setCurrentText(val==""?"640x360":val);
}

