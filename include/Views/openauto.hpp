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
#include "App/Worker.hpp"
#include "openauto/App.hpp"
#include "openauto/Configuration/Configuration.hpp"
#include "openauto/Configuration/IConfiguration.hpp"
#include "openauto/Configuration/RecentAddressesList.hpp"
#include "openauto/Service/AndroidAutoEntityFactory.hpp"
#include "openauto/Service/ServiceFactory.hpp"

class OpenAuto : public Worker {
    Q_OBJECT

   public:
    OpenAuto(std::function<void(bool)> callback, bool night_mode, QWidget *_frame);
    ~OpenAuto();

    inline void start() { app->waitForDevice(true); }
    inline void SetOpacity(unsigned int alpha) override { serviceFactory.setOpacity(alpha); }
    inline void UpdateSize() override { serviceFactory.resize(); }
    inline void SetNightMode(bool mode) override { serviceFactory.setNightMode(mode); }
    inline void SendKeyEvent(QKeyEvent *event) override{ serviceFactory.sendKeyEvent(event); }
    QWidget* Display() override;
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


