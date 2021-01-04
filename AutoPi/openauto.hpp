#ifndef OPENAUTO_HPP
#define OPENAUTO_HPP
#include <thread>
#include <QApplication>
#include <f1x/aasdk/USB/USBHub.hpp>
#include <f1x/aasdk/USB/ConnectedAccessoriesEnumerator.hpp>
#include <f1x/aasdk/USB/AccessoryModeQueryChain.hpp>
#include <f1x/aasdk/USB/AccessoryModeQueryChainFactory.hpp>
#include <f1x/aasdk/USB/AccessoryModeQueryFactory.hpp>
#include <f1x/aasdk/TCP/TCPWrapper.hpp>
#include <f1x/openauto/autoapp/App.hpp>
#include <f1x/openauto/autoapp/Configuration/IConfiguration.hpp>
#include <f1x/openauto/autoapp/Configuration/RecentAddressesList.hpp>
#include <f1x/openauto/autoapp/Service/AndroidAutoEntityFactory.hpp>
#include <f1x/openauto/autoapp/Service/ServiceFactory.hpp>
#include <f1x/openauto/autoapp/Configuration/Configuration.hpp>
#include <f1x/openauto/autoapp/UI/MainWindow.hpp>
#include <f1x/openauto/autoapp/UI/SettingsWindow.hpp>
#include <f1x/openauto/autoapp/UI/ConnectDialog.hpp>
#include <f1x/openauto/Common/Log.hpp>

namespace aasdk = f1x::aasdk;
namespace autoapp = f1x::openauto::autoapp;
using ThreadPool = std::vector<std::thread>;

void startUSBWorkers(boost::asio::io_service& ioService, libusb_context* usbContext, ThreadPool& threadPool)
{
    auto usbWorker = [&ioService, usbContext]() {
        timeval libusbEventTimeout{180, 0};

        while(!ioService.stopped())
        {
            libusb_handle_events_timeout_completed(usbContext, &libusbEventTimeout, nullptr);
        }
    };

    threadPool.emplace_back(usbWorker);
    threadPool.emplace_back(usbWorker);
    threadPool.emplace_back(usbWorker);
    threadPool.emplace_back(usbWorker);
}

void startIOServiceWorkers(boost::asio::io_service& ioService, ThreadPool& threadPool)
{
    auto ioServiceWorker = [&ioService]() {
        ioService.run();
    };

    threadPool.emplace_back(ioServiceWorker);
    threadPool.emplace_back(ioServiceWorker);
    threadPool.emplace_back(ioServiceWorker);
    threadPool.emplace_back(ioServiceWorker);
}
#endif // OPENAUTO_HPP
