#ifndef SETTINGSDLG_H
#define SETTINGSDLG_H

#include <QDialog>
#include "settingsmanager.h"

namespace Ui {
class CSettingsDlg;
}

class CSettingsDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit CSettingsDlg(QWidget *parent = 0);
    ~CSettingsDlg();

public slots:
    void onPbApplyClicked();

private slots:
    void on_wsSelectBkg_clicked();

private:
    Ui::CSettingsDlg *ui;
    SettingsManager m_setting;
    void fillWorksizes(QString val);

};

#endif // SETTINGSDLG_H
