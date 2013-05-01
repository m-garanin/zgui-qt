#ifndef STARTAIRDIALOG_H
#define STARTAIRDIALOG_H

#include <QDialog>

namespace Ui {
class StartAirDialog;
}

class StartAirDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit StartAirDialog(QWidget *parent = 0);
    ~StartAirDialog();
    bool test_mode;

protected:
    bool eventFilter(QObject *, QEvent *);
    
private slots:
    void on_startBtn_clicked();

    void saveValues();

    void on_testBtn_clicked();

private:
    Ui::StartAirDialog *ui;

    void startAir(int test);
    void fillLabels();
    void loadValues();
    void fillQuality();
};

#endif // STARTAIRDIALOG_H
