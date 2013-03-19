#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QSettings>
#include <QStringList>

class SettingsManager : public QObject
{
    Q_OBJECT
public:
    explicit SettingsManager(QString prefix, QString settingsFileName = QString(), QObject *parent = 0);
    ~SettingsManager();

    static void setGlobalSettingsFilePath(QString filePath);
    static QString globalSettingsFilePath();

    void setValue(QString key, QVariant value, QString prefix = QString());
    QVariant getValue(QString key, QString prefix = QString());
    void setList(QString key, QStringList list, QString prefix = QString());
    QStringList getList(QString key, QString prefix = QString());

    int getIntValue(QString key, QString prefix = QString());
    bool getBoolValue(QString key, QString prefix = QString());
    QString getStringValue(QString key, QString prefix = QString());

private:
    QSettings * _settings;
    QString _prefix;

    static QString _globalSettingsFilePath;

    QString setPrefix(QString key, QString prefix);

signals:
    
public slots:
    
};

#endif // SETTINGSMANAGER_H
