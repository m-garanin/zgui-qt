#ifndef UTILS_H
#define UTILS_H

#include <QStringList>

QStringList getVideoCaptureDevices();
QStringList getAudioCaptureDevices();

QString friendlyDeviceName(QString);

QString getWorksize();
bool getAutoSaveRestore();
#endif // UTILS_H
