#include "settingsdlg.h"
#include "ui_settingsdlg.h"
#include "settingsmanager.h"
#include "utils.h"
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
    fillWorksizes(getWorksize());
    fillWorkpattern(setting.getIntValue("Workpattern"));

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

    // workspace pattern
    int ptr = ui->workspacePatternComboBox->itemData(ui->workspacePatternComboBox->currentIndex()).toInt();
    if(ptr != setting.getIntValue("Workpattern")){
        setting.setValue("Workpattern", ptr);
        global_manager->setBackground(ptr);
    }

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

void CSettingsDlg::fillWorkpattern(int val)
{
    QComboBox* bx = ui->workspacePatternComboBox;
    bx->addItem("A standard SMPTE test pattern", 0);
    bx->addItem("SMPTE test pattern (100% color bars)",19);
    bx->addItem("Random noise", 1);
    bx->addItem("A black image", 2 );
    bx->addItem("A white image", 3);
    bx->addItem("A red image", 4);
    bx->addItem("A green image", 5);
    bx->addItem("A blue image", 6);    
    bx->addItem("Circular pattern", 11);

    /*
    bx->addItem("", );
    */
    int ind = bx->findData(val);
    bx->setCurrentIndex(ind);
}

