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
    QString getPreferableSize() {return m_psize;};


public slots:
    void onSelect();
private:
    Ui::CaptureSelectDialog *ui;    
    QString m_device, m_psize;
    void fillDevices(CaptureDeviceType type);
    void fillPSize();

    QStringList getDualAudioCaptureDevices();
};

#endif // CAPTURESELECTDIALOG_H
