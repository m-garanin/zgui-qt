#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

#include "settingsdlg.h"
#include "ui_settingsdlg.h"
#include "utils.h"
#include "IManager.h"

CSettingsDlg::CSettingsDlg(QWidget *parent) :
    QDialog(parent),
    m_setting("Settings"),
    ui(new Ui::CSettingsDlg)
{
    ui->setupUi(this);

    // TODO: может вообще убрать ...
    ui->lblProf->setVisible(false);
    ui->chkProfMode->setVisible(false);

    //qDebug() << "QGLFormat::hasOpenGLOverlays() = " << QGLFormat::hasOpenGLOverlays();
    //qDebug() << "QGLFormat::openGLVersionFlags() = " << QGLFormat::openGLVersionFlags();

    connect(ui->pbApply, SIGNAL(clicked()), SLOT(onPbApplyClicked()));
    connect(ui->pbClose, SIGNAL(clicked()), SLOT(reject()));
    //SettingsManager setting("Settings");

    /*
    if(QGLFormat::hasOpenGL())
        ui->cbOpenGL->setChecked(setting.getBoolValue("OpenGL"));
    else
        ui->cbOpenGL->setEnabled(false);
    */
    fillWorksizes(getWorksize());

    ui->wsBkg->setText( m_setting.getStringValue("Background") );
    ui->chkAutoSaveRestore->setChecked(getAutoSaveRestore());
    ui->chkProfMode->setChecked(getProfMode());
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

    if(wsize != getWorksize()){
        setting.setValue("Worksize", wsize );
        QStringList sz = wsize.split("x");
        uint w = sz[0].toInt();
        uint h = sz[1].toInt();
        //qDebug() << "WORKSIZE CHANGE" << w << "x" << h;
        global_manager->setWorksize(w, h);
    }

    QString bkg = ui->wsBkg->text();
    global_manager->setBackground(bkg);

    setting.setValue("Background", bkg);
    setting.setValue("AutoSaveRestore", ui->chkAutoSaveRestore->isChecked());
    setting.setValue("ProfMode", ui->chkProfMode->isChecked());


    accept();
}

void CSettingsDlg::fillWorksizes(QString val)
{
    QStringList sizes;
    sizes  << "320x240"
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
           << "960x540"
           << "1024x576"
           << "1280x720"
           << "1366x768"
           << "1600x900"
           << "1920x1080";

    ui->workspaceSizeComboBox->addItems(sizes);
    ui->workspaceSizeComboBox->setCurrentText(val==""?"640x360":val);
}


void CSettingsDlg::on_wsSelectBkg_clicked()
{

    QString file = QFileDialog::getOpenFileName(this, tr("Select Background Image"),
                                                m_setting.getStringValue("Background"), "Image Files (*.png *.jpg)");
    if (!file.isEmpty())
    {
        ui->wsBkg->setText(file);
    }

}
