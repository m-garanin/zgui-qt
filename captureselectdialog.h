#ifndef CAPTURESELECTDIALOG_H
#define CAPTURESELECTDIALOG_H

#include <QDialog>

namespace Ui {
class CaptureSelectDialog;
}

class CaptureSelectDialog : public QDialog
{
    Q_OBJECT
    
public:
    enum CaptureDeviceType {
        VideoDevice,
        AudioDevice
    };

    explicit CaptureSelectDialog(CaptureDeviceType type, QWidget *parent = 0);
    ~CaptureSelectDialog();
    
    QString getDevice() {return m_device;};

public slots:
    void onSelect();
private:
    Ui::CaptureSelectDialog *ui;    
    QString m_device;
    void fillDevices(CaptureDeviceType type);

};

#endif // CAPTURESELECTDIALOG_H
