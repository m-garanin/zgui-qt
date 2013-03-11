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

    QString selectedEffectName() const;
    void setColuntCount(quint32 count);

public slots:
    void onPbOkClicked();
    void onPbCancelClicked();
    void onPbEffectClicked();

private:
    void loadEffectsImg();
    
private:
    Ui::CEffectsDlg *ui;

    QString selected_effect;
    int column_count;
};

#endif // EFFECTSDLG_H
