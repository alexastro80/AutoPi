#pragma once

#include "openauto/Configuration/Configuration.hpp"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QFrame>
#include <QKeySequence>
#include <QPluginLoader>
#include <QObject>
#include <QSettings>
#include <QString>
#include <QWidget>
#include <QVideoFrame>

class Config : public QObject {
    Q_OBJECT

   public:
    Config();
    ~Config();


    void save();

    bool DarkMode;
    
    inline int Volume() { return volume; }
    inline void Volume(int vol)
        {   volume = std::max(0, std::min(vol, 100));
        //   UpdateSystemVolume();
            emit volumeChanged(volume);
        }
    
    inline int Brightness() { return brightness; }
    inline void Brightness(int bright)
        {
            brightness = std::max(76, std::min(bright, 255));
            emit brightnessChanged(brightness);
        }

    inline bool SIUnits() { return siUnits; }
    inline void SIUnits(bool si)
        {
            siUnits = si;
            emit siUnitsChanged(siUnits);
        }
    QString ColorLight;
    QString ColorDark;
    
    inline QColor Color() { return QColor(darkMode ? ColorDark : ColorLight); }
    inline void Color(QColor color)
        {
            if(darkMode)
                ColorDark = color.name();
            else
                ColorLight = color.name();
        }
    QString BluetoothDevice;
    double RadioStation;
    bool RadioMuted;
    bool WirelessActive;
    QString WirelessAddress;
    QString LauncherApp;
    bool MouseActive;
    inline void SetShortcut(QString id, QString _short) { shortcuts[id] = _short; }
    inline QString GetShortcut(QString id) { return shortcuts[id]; }

    inline QString QuickView() { return quickView; }
    inline void QuickView(QString qv)
        {
            quickView = qv;
            emit quickViewChanged(quickView);
        }
    inline QMap<QString, QWidget *> GetQuickViews() { return quickViews; }
    inline QWidget *GetQuickView(QString name) { return quickViews[name]; }
    inline void AddQuickView(QString name, QWidget *view) { quickViews[name] = view; }

    double Scale;
    

    inline bool Page(QWidget *page) { return pages.value(page->objectName(), true); }
    inline void Page(QWidget *page, bool enabled)
        {
            pages[page->objectName()] = enabled;
            emit pageChanged(page, enabled);
        }

    std::shared_ptr<openauto::configuration::Configuration> openautoConfig;
    openauto::configuration::Configuration::ButtonCodes openautoButtonCodes;

    static Config *getInstance();

   private:
    QMap<QString, QWidget *> quickViews;

    QSettings settings;
    int volume;
    bool darkMode;
    int brightness;
    bool siUnits;
    bool mouseActive;
    QMap<QString, QString> shortcuts;
    QString quickView;


    QMap<QString, bool> pages;

    void UpdateSystemVolume();

   signals:
    void volumeChanged(int volume);
    void brightnessChanged(unsigned int brightness);
    void siUnitsChanged(bool siUnits);
    void scaleChanged(double scale);
    void quickViewChanged(QString quickView);
    void pageChanged(QWidget *page, bool enabled);

};
