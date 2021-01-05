#ifndef OPENAUTOTAB_HPP
#define OPENAUTOTAB_HPP

#include <QtWidgets>
#include <f1x/aasdk/TCP/TCPWrapper.hpp>
#include <f1x/aasdk/USB/AccessoryModeQueryChain.hpp>
#include <f1x/aasdk/USB/AccessoryModeQueryChainFactory.hpp>
#include <f1x/aasdk/USB/AccessoryModeQueryFactory.hpp>
#include <f1x/aasdk/USB/ConnectedAccessoriesEnumerator.hpp>
#include <f1x/aasdk/USB/USBHub.hpp>

#include <f1x/openauto/autoapp/App.hpp>		
#include <f1x/openauto/autoapp/Configuration/Configuration.hpp>
#include <f1x/openauto/autoapp/Configuration/IConfiguration.hpp>
#include <f1x/openauto/autoapp/Configuration/RecentAddressesList.hpp>
#include <f1x/openauto/autoapp/Service/AndroidAutoEntityFactory.hpp>
#include <f1x/openauto/autoapp/Service/ServiceFactory.hpp>

namespace autoapp = f1x::openauto::autoapp;
class OpenAutoWorker {
public:
    OpenAutoWorker(std::function<void(bool)> callback = nullptr, QWidget *parent = nullptr);
    ~OpenAutoWorker();

    inline void start() { this->app->waitForUSBDevice(); }
    inline void set_Opacity(unsigned int alpha) { this->service_factory.setOpacity(alpha); }

private:
    void create_usb_workers();
    void create_io_service_workers();

    libusb_context *usb_context;
    boost::asio::io_service                                 io_service;
    boost::asio::io_service::work                           work;
    std::shared_ptr<autoapp::configuration::Configuration>  configuration;
    f1x::aasdk::tcp::TCPWrapper                             tcpWrapper;
    f1x::aasdk::usb::USBWrapper                             usbWrapper;
    f1x::aasdk::usb::AccessoryModeQueryFactory              queryFactory;
    f1x::aasdk::usb::AccessoryModeQueryChainFactory         queryChainFactory;
    autoapp::service::ServiceFactory                        serviceFactory;
    autoapp::service::AndroidAutoEntityFactory              androidAutoEntityFactory;
    std::shared_ptr<f1x::aasdk::usb::USBHub>                usbHub;
    std::shared_ptr<f1x::aasdk::usb::ConnectedAccessoriesEnumerator> connectedAccessoriesEnum;
    std::shared_ptr<autoapp::App>                           app;
    std::vector<std::thread>                                threadPool;
};

class OpenAutoView :public QWidget {
    Q_OBJECT

public:
    OpenAutoView(QWidget *parent = nullptr);
    ~OpenAutoView();

    void startWorker();

private:
    QWidget* msgWidget)();

    OpenAutoWorker *worker = nullptr;
};

#endif // OPENAUTOTAB_HPP;

