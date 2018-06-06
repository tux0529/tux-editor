#include "settings.h"

Settings *Settings::s_instance = NULL;

const QString settingFileName = "tuxeditor.conf";

Settings::Settings(QObject *parent) : QObject(parent)
{
    initSettings();
}

void Settings::initSettings()
{
    QSettings settings(settingFilePath(), QSettings::IniFormat);
    m_wallpaperDirPath = settings.value( "WallpaperDirPath", "E:/redtux/seafile/Website/tux.red/files/wallpaper").toString();
}

QString Settings::wallpaperDirPath()
{
    return instance()->m_wallpaperDirPath;
}

void Settings::setWallpaperDirPath(const QString &wdp)
{
    instance()->setValue("WallpaperDirPath", wdp);
}

QString Settings::settingFilePath()
{
    return QString("D:/%1").arg(settingFileName);
}
