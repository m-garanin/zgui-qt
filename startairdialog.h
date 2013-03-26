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

protected:
    bool eventFilter(QObject *, QEvent *);
    
private slots:
    void on_startBtn_clicked();

    void saveValues();

private:
    Ui::StartAirDialog *ui;

    void fillLabels();
    void loadValues();
};

#endif // STARTAIRDIALOG_H
