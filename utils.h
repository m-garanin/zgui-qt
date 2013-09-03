#ifndef UTILS_H
#define UTILS_H

#include <QStringList>

QStringList getVideoCaptureDevices();
QStringList getAudioCaptureDevices();

QString friendlyDeviceName(QString);
#endif // UTILS_H
