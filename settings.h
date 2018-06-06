#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>


class Settings : public QObject
{
    Q_OBJECT
public:
    static QString wallpaperDirPath();
    static void setWallpaperDirPath(const QString &wdp);

signals:

public slots:

private:
    static Settings *s_instance;

    static Settings *instance();

    static QString settingFilePath();

    Settings(QObject *parent = 0);

    void initSettings();
    void setValue(const QString &key, const QVariant &value);
    QString m_wallpaperDirPath;
};

inline Settings *Settings::instance()
{
    if(!s_instance)
        s_instance = new Settings();
    return s_instance;
}

inline void Settings::setValue(const QString &key, const QVariant &value)
{
    QSettings settings(settingFilePath(), QSettings::IniFormat);
    settings.setValue(key, value);
}

#endif // SETTINGS_H
