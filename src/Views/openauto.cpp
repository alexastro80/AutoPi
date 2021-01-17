#include "Views/openauto.hpp"

#include "App/config.hpp"
#include "App/theme.hpp"
//#include "App/widgets/ip_input.hpp"
//#include "App/widgets/progress.hpp"
//#include "App/window.hpp"

OpenAuto::OpenAuto(std::function<void(bool)> callback, bool night_mode, QWidget *frame)
    : Worker(night_mode, frame),
      io_service(),
      work(io_service),
      configuration(Config::getInstance()->openautoConfig),
      tcpWrapper(),
      usbWrapper((libusb_init(&usb_context), usb_context)),
      queryFactory(usbWrapper, io_service),
      queryChainFactory(usbWrapper, io_service, queryFactory),
      serviceFactory(io_service, configuration, frame, callback, night_mode),
      androidAutoEntityFactory(io_service, configuration, serviceFactory),
      USBHub(std::make_shared<aasdk::usb::USBHub>(usbWrapper, io_service, queryChainFactory)),
      connectedAccessoriesEnum(
          std::make_shared<aasdk::usb::ConnectedAccessoriesEnumerator>(usbWrapper, io_service, queryChainFactory)),
      app(std::make_shared<openauto::App>(io_service, usbWrapper, tcpWrapper, androidAutoEntityFactory, USBHub,
                                          connectedAccessoriesEnum))
{
    createUsbWorkers();
    createIOServiceWorkers();

    app->waitForDevice(true);
}

OpenAuto::~OpenAuto()
{
    std::for_each(threadPool.begin(), threadPool.end(),
                  std::bind(&std::thread::join, std::placeholders::_1));
    libusb_exit(usb_context);
}

void OpenAuto::createUsbWorkers()
{
    std::function<void()> worker = [this]() {
        timeval event_timeout = {180, 0};
        while (!io_service.stopped())
            libusb_handle_events_timeout_completed(usb_context, &event_timeout, nullptr);
    };

    threadPool.emplace_back(worker);
    threadPool.emplace_back(worker);
    threadPool.emplace_back(worker);
    threadPool.emplace_back(worker);
}

void OpenAuto::createIOServiceWorkers()
{
    std::function<void()> worker = [this]() { io_service.run(); };

    threadPool.emplace_back(worker);
    threadPool.emplace_back(worker);
    threadPool.emplace_back(worker);
    threadPool.emplace_back(worker);
}

QWidget* OpenAuto::Display()
{
    QWidget* widget = new QWidget(frame);
    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);

    QLabel *label = new QLabel("connect device to start Android Auto", widget);
    label->setAlignment(Qt::AlignCenter);

    layout->addWidget(label);
    layout->addStretch();

    return widget;
}


