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
    
private slots:
    void on_startBtn_clicked();

private:
    Ui::StartAirDialog *ui;

    void fillLabels();
};

#endif // STARTAIRDIALOG_H
