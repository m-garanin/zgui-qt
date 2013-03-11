#ifndef RECORDDIALOG_H
#define RECORDDIALOG_H

#include <QDialog>

namespace Ui {
class RecordDialog;
}

class RecordDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit RecordDialog(QWidget *parent = 0);
    ~RecordDialog();
    
private:
    Ui::RecordDialog *ui;
};

#endif // RECORDDIALOG_H
