#include <unistd.h>
#include <BluezQt/Adapter>
#include <BluezQt/Device>
#include <BluezQt/InitManagerJob>
#include <BluezQt/Manager>
#include <BluezQt/MediaPlayer>
#include <BluezQt/MediaPlayerTrack>
#include <QAbstractSlider>
#include <QApplication>
#include <QBluetoothAddress>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QBluetoothLocalDevice>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothServiceInfo>
#include <QTimer>

#include "App/bluetooth.hpp"
//#include "App/widgets/progress.hpp"

Bluetooth::Bluetooth() : QObject(qApp)
{
    BluezQt::Manager *manager = new BluezQt::Manager();
    BluezQt::InitManagerJob *job = manager->init();
    job->exec();

    adapter = manager->usableAdapter();

    scanTimer = new QTimer(this);
    scanTimer->setSingleShot(true);
    connect(scanTimer, &QTimer::timeout, [this]() { StopScan(); });

    if (HasAdapter()) {
        for (auto device : GetDevices()) {
            if (device->mediaPlayer() != nullptr) {
                mediaPlayerDevice = device;
                break;
            }
        }

        connect(adapter.data(), &BluezQt::Adapter::deviceAdded,
                [this](BluezQt::DevicePtr device) { emit deviceAdded(device); });
        connect(adapter.data(), &BluezQt::Adapter::deviceChanged, [this](BluezQt::DevicePtr device) {
            emit deviceChanged(device);
            updateMediaPlayer(device);
        });
        connect(adapter.data(), &BluezQt::Adapter::deviceRemoved,
                [this](BluezQt::DevicePtr device) { emit deviceRemoved(device); });
    }
}

void Bluetooth::StartScan()
{
    if (HasAdapter()) {
        if (!adapter->isDiscovering()) {
            emit scanStatus(true);
            adapter->startDiscovery();
            scanTimer->start(15000);
        }
    }
}

void Bluetooth::StopScan()
{
    if (HasAdapter()) {
        if (adapter->isDiscovering()) {
            emit scanStatus(false);
            adapter->stopDiscovery();
        }
    }
}

void Bluetooth::updateMediaPlayer(BluezQt::DevicePtr device)
{
    if (device->mediaPlayer() != nullptr) {
        emit mediaPlayerStatusChanged(device->mediaPlayer()->status());
        emit mediaPlayerTrackChanged(device->mediaPlayer()->track());
        emit mediaPlayerChanged(device->name(), device->mediaPlayer());
        mediaPlayerDevice = device;
    }
    else if (mediaPlayerDevice == device) {
        emit mediaPlayerStatusChanged(BluezQt::MediaPlayer::Status::Paused);
        emit mediaPlayerTrackChanged(BluezQt::MediaPlayerTrack());
        emit mediaPlayerChanged(QString(), QSharedPointer<BluezQt::MediaPlayer>(nullptr));
    }
}

Bluetooth *Bluetooth::getInstance()
{
    static Bluetooth bluetooth;
    return &bluetooth;
}
