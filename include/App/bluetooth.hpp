#pragma once

#include <BluezQt/Adapter>
#include <BluezQt/Device>
#include <BluezQt/MediaPlayer>
#include <BluezQt/MediaPlayerTrack>
#include <QList>
#include <QMainWindow>
#include <QMap>
#include <QObject>
#include <QPair>
#include <QPushButton>
#include <QString>

//#include "app/widgets/progress.hpp"

class Bluetooth : public QObject {
    Q_OBJECT

   public:
    Bluetooth();

    void StartScan();
    void StopScan();

    inline QList<BluezQt::DevicePtr> GetDevices()
    {
        return HasAdapter() ? adapter->devices() : QList<BluezQt::DevicePtr>();
    }
    inline QPair<QString, BluezQt::MediaPlayerPtr> GetMediaPlayer()
    {
        if (mediaPlayerDevice != nullptr)
            return {mediaPlayerDevice->name(), mediaPlayerDevice->mediaPlayer()};

        return {QString(), QSharedPointer<BluezQt::MediaPlayer>(nullptr)};
    }
    inline bool HasAdapter() { return adapter != nullptr; }

    static Bluetooth *getInstance();

   private:
    void updateMediaPlayer(BluezQt::DevicePtr device);

    BluezQt::AdapterPtr adapter;
    BluezQt::DevicePtr mediaPlayerDevice;
    QTimer *scanTimer;

   signals:
    void deviceAdded(BluezQt::DevicePtr);
    void deviceChanged(BluezQt::DevicePtr);
    void deviceRemoved(BluezQt::DevicePtr);
    void mediaPlayerChanged(QString, BluezQt::MediaPlayerPtr);
    void mediaPlayerStatusChanged(BluezQt::MediaPlayer::Status);
    void mediaPlayerTrackChanged(BluezQt::MediaPlayerTrack);
    void scanStatus(bool);
};

