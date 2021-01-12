#include <QApplication>
#include <QDir>
#include <QTimer>
#include <QProcess>

#include <App/config.hpp>
//#include "plugins/brightness_plugin.hpp"

Config::Config()
    : QObject(qApp),
      openautoConfig(std::make_shared<openauto::configuration::Configuration>()),
      openautoButtonCodes(openautoConfig->getButtonCodes()),
      settings(QSettings::IniFormat, QSettings::UserScope, "dash")
{
 

    volume = settings.value("volume", 50).toInt();
    DarkMode = settings.value("DarkMode", false).toBool();
    brightness = settings.value("brightness", 255).toInt();
    siUnits = settings.value("siUnits", false).toBool();
    ColorLight = settings.value("ColorLight", "#000000").toString();
    ColorDark = settings.value("ColorDark", "#ffffff").toString();
    BluetoothDevice = settings.value("Bluetooth/device", QString()).toString();
    RadioStation = settings.value("Radio/station", 98.0).toDouble();
    RadioMuted = settings.value("Radio/muted", true).toBool();
    WirelessActive = settings.value("Wireless/active", false).toBool();
    WirelessAddress = settings.value("Wireless/address", "0.0.0.0").toString();
    MouseActive = settings.value("MouseActive", true).toBool();
    quickView = settings.value("quickView", "none").toString();
    Scale = settings.value("Scale", 1.0).toDouble();
    settings.beginGroup("Pages");
    for (auto key : settings.childKeys())
        pages[key] = settings.value(key, true).toBool();
    settings.endGroup();
    settings.beginGroup("Shortcuts");
    for (auto key : settings.childKeys())
        shortcuts[key] = settings.value(key, QString()).toString();
    settings.endGroup();
}

Config::~Config()
{
    save();
}

void Config::save()
{
    if (volume != settings.value("volume", 50).toInt())
        settings.setValue("volume", volume);
    if (DarkMode != settings.value("DarkMode", false).toBool())
        settings.setValue("DarkMode", DarkMode);
    if (brightness != settings.value("brightness", 255).toInt())
        settings.setValue("brightness", brightness);
    if (siUnits != settings.value("siUnits", false).toBool())
        settings.setValue("siUnits", siUnits);
    if (ColorLight != settings.value("ColorLight", "#000000").toString())
        settings.setValue("ColorLight", ColorLight);
    if (ColorDark != settings.value("ColorDark", "#ffffff").toString())
        settings.setValue("ColorDark", ColorDark);
    if (BluetoothDevice != settings.value("Bluetooth/device", QString()).toString())
        settings.setValue("Bluetooth/device", BluetoothDevice);
    if (RadioStation != settings.value("Radio/station", 98.0).toDouble())
        settings.setValue("Radio/station", RadioStation);
    if (RadioMuted != settings.value("Radio/muted", true).toBool())
        settings.setValue("Radio/muted", RadioMuted);
    if (WirelessActive != settings.value("Wireless/active", false).toBool())
        settings.setValue("Wireless/active", WirelessActive);
    if (WirelessAddress != settings.value("Wireless/address", "0.0.0.0").toString())
        settings.setValue("Wireless/address", WirelessAddress);
    if (MouseActive != settings.value("MouseActive", true).toBool())
        settings.setValue("MouseActive", MouseActive);
    if (quickView != settings.value("quickView", "volume").toString())
        settings.setValue("quickView", quickView);
    if (Scale != settings.value("Scale", 1.0).toDouble())
        settings.setValue("Scale", Scale);
for (auto id : pages.keys()) {
        QString configKey = QString("Pages/%1").arg(id);
        bool page_enabled = pages[id];
        if (page_enabled != settings.value(configKey, true).toBool())
            settings.setValue(configKey, page_enabled);
    }
    for (auto id : shortcuts.keys()) {
        QString configKey = QString("Shortcuts/%1").arg(id);
        QString shortcut = shortcuts[id];
        if (shortcut != settings.value(configKey, QString()).toString())
            settings.setValue(configKey, shortcut);
    }

    settings.sync();
}

Config *Config::getInstance()
{
    static Config config;
    return &config;
}


/*
void Config::Update_system_volume()
{
    static QString command("amixer set Master %1% --quiet");

    QProcess *lProc = new QProcess();
    lProc->start(command.arg(volume));
    lProc->waitForFinished();
}
*/

