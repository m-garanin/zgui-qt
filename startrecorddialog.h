#ifndef STARTRECORDDIALOG_H
#define STARTRECORDDIALOG_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
class StartRecordDialog;
}

class StartRecordDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit StartRecordDialog(QWidget *parent = 0);
    ~StartRecordDialog();
    
private slots:
    void on_selectFolderBtn_clicked();

private:
    Ui::StartRecordDialog *ui;

    void fillLabels();
};

#endif // STARTRECORDDIALOG_H
