#include "settingsdlg.h"
#include "ui_settingsdlg.h"
#include "settingsmanager.h"

#include <QGLFormat>

CSettingsDlg::CSettingsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSettingsDlg)
{
    ui->setupUi(this);

    qDebug() << "QGLFormat::hasOpenGLOverlays() = " << QGLFormat::hasOpenGLOverlays();
    qDebug() << "QGLFormat::openGLVersionFlags() = " << QGLFormat::openGLVersionFlags();

    connect(ui->pbApply, SIGNAL(clicked()), SLOT(onPbApplyClicked()));
    connect(ui->pbClose, SIGNAL(clicked()), SLOT(reject()));
    SettingsManager setting("Video");
    if(QGLFormat::hasOpenGL())
        ui->cbOpenGL->setChecked(setting.getBoolValue("OpenGL"));
    else
        ui->cbOpenGL->setEnabled(false);
}

CSettingsDlg::~CSettingsDlg()
{
    delete ui;
}

void CSettingsDlg::onPbApplyClicked()
{
    SettingsManager setting("Video");
    setting.setValue("OpenGL", ui->cbOpenGL->isChecked());
    accept();
}

