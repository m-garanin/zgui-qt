#include "settingsmanager.h"
#include <QDebug>

QString SettingsManager::_globalSettingsFilePath = "";

SettingsManager::SettingsManager(QString prefix, QString settingsFilePath, QObject *parent) :
    QObject(parent)
{
    _prefix = prefix;

    QString filePath;
    if(settingsFilePath.length() > 0)
        filePath = settingsFilePath;
    else
        filePath = _globalSettingsFilePath;

    _settings = new QSettings(filePath, QSettings::IniFormat, parent);
}

SettingsManager::~SettingsManager()
{
    delete _settings;
}

void SettingsManager::setValue(QString key, QVariant value, QString prefix)
{
    _settings->setValue(setPrefix(key, prefix), value);
}

QVariant SettingsManager::getValue(QString key, QString prefix)
{
    return _settings->value(setPrefix(key, prefix));
}

void SettingsManager::setList(QString key, QStringList list, QString prefix)
{
    _settings->setValue(setPrefix(key, prefix), list);
}

QStringList SettingsManager::getList(QString key, QString prefix)
{
    QVariant list = _settings->value(setPrefix(key, prefix));
    return list.toStringList();
}

int SettingsManager::getIntValue(QString key, QString prefix)
{
    return this->getValue(key, prefix).toInt();
}

bool SettingsManager::getBoolValue(QString key, QString prefix)
{
    return this->getValue(key, prefix).toBool();
}

QString SettingsManager::getStringValue(QString key, QString prefix)
{
    return this->getValue(key, prefix).toString();
}

QString SettingsManager::setPrefix(QString key, QString prefix)
{
    if(prefix.length() == 0)
        prefix = _prefix;

    return key.prepend("/").prepend(prefix);
}

void SettingsManager::setGlobalSettingsFilePath(QString filePath)
{
    SettingsManager::_globalSettingsFilePath = filePath;
}

QString SettingsManager::globalSettingsFilePath()
{
    return _globalSettingsFilePath;
}
