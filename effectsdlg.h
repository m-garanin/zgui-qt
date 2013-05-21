#ifndef EFFECTSDLG_H
#define EFFECTSDLG_H

#include <QDialog>

namespace Ui {
class CEffectsDlg;
}

class CEffectsDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit CEffectsDlg(QWidget *parent = 0);
    ~CEffectsDlg();

    QString getEffect() {return m_effect;};

public slots:
    void onSelect();
    
private slots:
    void on_cleanButton_clicked();

private:
    Ui::CEffectsDlg *ui;
    QString m_effect;
    void fill();
};

#endif // EFFECTSDLG_H
