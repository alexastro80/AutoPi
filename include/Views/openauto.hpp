#pragma once

#include <QtWidgets>
#include <thread>

#include "aasdk/TCP/TCPWrapper.hpp"
#include "aasdk/USB/AccessoryModeQueryChain.hpp"
#include "aasdk/USB/AccessoryModeQueryChainFactory.hpp"
#include "aasdk/USB/AccessoryModeQueryFactory.hpp"
#include "aasdk/USB/ConnectedAccessoriesEnumerator.hpp"
#include "aasdk/USB/USBHub.hpp"
#include "App/config.hpp"
#include "App/bluetooth.hpp"
//#include "app/widgets/switch.hpp"
//#include "app/widgets/dialog.hpp"
#include "App/theme.hpp"
#include "openauto/App.hpp"
#include "openauto/Configuration/Configuration.hpp"
#include "openauto/Configuration/IConfiguration.hpp"
#include "openauto/Configuration/RecentAddressesList.hpp"
#include "openauto/Service/AndroidAutoEntityFactory.hpp"
#include "openauto/Service/ServiceFactory.hpp"

class OpenAutoWorker : public QObject {
    Q_OBJECT

   public:
    OpenAutoWorker(std::function<void(bool)> callback, bool night_mode, QWidget *frame);
    ~OpenAutoWorker();

    inline void start() { app->waitForDevice(true); }
    inline void SetOpacity(unsigned int alpha) { serviceFactory.setOpacity(alpha); }
    inline void UpdateSize() { serviceFactory.resize(); }
    inline void SetNightMode(bool mode) { serviceFactory.setNightMode(mode); }
    inline void SendKeyEvent(QKeyEvent *event) { serviceFactory.sendKeyEvent(event); }

   private:
    void createUsbWorkers();
    void createIOServiceWorkers();

    libusb_context *usb_context;
    boost::asio::io_service io_service;
    boost::asio::io_service::work work;
    std::shared_ptr<openauto::configuration::Configuration> configuration;
    aasdk::tcp::TCPWrapper tcpWrapper;
    aasdk::usb::USBWrapper usbWrapper;
    aasdk::usb::AccessoryModeQueryFactory queryFactory;
    aasdk::usb::AccessoryModeQueryChainFactory queryChainFactory;
    openauto::service::ServiceFactory serviceFactory;
    openauto::service::AndroidAutoEntityFactory androidAutoEntityFactory;
    std::shared_ptr<aasdk::usb::USBHub> USBHub;
    std::shared_ptr<aasdk::usb::ConnectedAccessoriesEnumerator> connectedAccessoriesEnum;
    std::shared_ptr<openauto::App> app;
    std::vector<std::thread> threadPool;
};

class OpenAutoFrame : public QWidget {
    Q_OBJECT

   public:
    OpenAutoFrame(QWidget *parent) : QWidget(parent) {}

    inline bool IsFullscreen() { return fullscreen; }
    inline void ToggleFullscreen() { fullscreen = !fullscreen; }

   protected:
    void mouseDoubleClickEvent(QMouseEvent *);
    inline void enterEvent(QEvent *) { setFocus(); }

   private:
    bool fullscreen = false;

   signals:
    void doubleClicked(bool fullscreen);
    void toggle(bool enable);
};

class OpenAutoView : public QStackedWidget {
    Q_OBJECT

   public:
    OpenAutoView(QWidget *parent = nullptr);

    inline void PassKeyEvent(QKeyEvent *event) { worker->SendKeyEvent(event); }

   protected:
    void resizeEvent(QResizeEvent *event);

   private:
    class Settings : public QWidget {
       public:
        Settings(QWidget *parent = nullptr);

       private:
        QWidget *settingsWidget();
        QBoxLayout *rhdRowWidget();
        QBoxLayout *frameRateRowWidget();
        QBoxLayout *resolutionRowWidget();
        QBoxLayout *dpiRowWidget();
        QBoxLayout *dpiWidget();
        QBoxLayout *rtAudioRowwidget();
        QBoxLayout *audioChannelsRowWidget();
        QBoxLayout *bluetoothRowWidget();
        QBoxLayout *touchscreenRowWidget();
        QCheckBox *buttonCheckbox(QString name, QString key, aasdk::proto::enums::ButtonCode::Enum code);
        QBoxLayout *buttonsRowWidget();

        Bluetooth *bluetooth;
        Config *config;
        Theme *theme;
    };

    QWidget *connectMsg();

    Config *config;
    Theme *theme;
    OpenAutoFrame *frame;
    OpenAutoWorker *worker;

   signals:
    void toggleFullscreen(QWidget *widget);
};
