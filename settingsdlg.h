#ifndef SETTINGSDLG_H
#define SETTINGSDLG_H

#include <QDialog>

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

private:
    Ui::CSettingsDlg *ui;
    void fillWorksizes(QString val);
};

#endif // SETTINGSDLG_H
