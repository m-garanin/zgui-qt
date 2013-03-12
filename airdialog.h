#ifndef AIRDIALOG_H
#define AIRDIALOG_H

#include <QDialog>

namespace Ui {
class AirDialog;
}

class AirDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AirDialog(QWidget *parent = 0);
    ~AirDialog();
    
private:
    Ui::AirDialog *ui;
};

#endif // AIRDIALOG_H
