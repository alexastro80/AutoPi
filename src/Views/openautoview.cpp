//#include "../../include/Views/openautotab.hpp"
#include "Views/openAutoView.hpp"
//#include "Views/openautotab.hpp"

OpenAutoWorker::OpenAutoWorker(std::function<void(bool)> callback, QWidget *parent)
    : io_service(),
      work(io_service),
//      configuration(Config::get_instance()->open_auto_config),
      tcpWrapper(),
      usbWrapper((libusb_init(&this->usb_context), usb_context)),
      queryFactory(usbWrapper,io_service),
      queryChainFactory(usbWrapper, io_service, queryFactory),
      serviceFactory(io_service, configuration, parent, callback),
      androidAutoEntityFactory(io_service, configuration, parent, callback),
      usbHub(std::make_shared<f1x::aasdk::usb::USBHub>(this->usbWrapper, this->io_service, this->queryChainFactory)),
      connectedAccessoriesEnum(std::make_shared<f1x::aasdk::usb::ConnectedAccessoriesEnumerator>(
        this->usbWrapper, this->io_service, this->queryChainFactory)),
      app(std::make_shared<autoapp::App>(this->io_service, this->usbWrapper, this->tcpWrapper,
                                          this->androidAutoEntityFactory, usbHub, connectedAccessoriesEnum))
{
  this->create_usb_workers();
  this->create_io_service_workers();
}                                   

OpenAutoWorker::~OpenAutoWorker()
{
  std::for_each(this->threadPool.begin(), this->threadPool.end(),
                std::bind(&std::thread::join, std::placeholders::_1));
  libusb_exit(this->usb_context);
}

void OpenAutoWorker::create_usb_workers()
{
  auto worker = [this]() {
    timeval event_timeout = {180, 0};
    while (!this->io_service.stopped())
      libusb_handle_events_timeout_completed(this->usb_context, &event_timeout,  nullptr);

  };

  this->threadPool.emplace_back(worker);
  this->threadPool.emplace_back(worker);
  this->threadPool.emplace_back(worker);
  this->threadPool.emplace_back(worker);
}

void OpenAutoWorker::create_io_service_workers()
{
  auto worker = [this]() { this->io_service.run();};
  
  this->threadPool.emplace_back(worker);
  this->threadPool.emplace_back(worker);
  this->threadPool.emplace_back(worker);
  this->threadPool.emplace_back(worker);
}

OpenAutoView::OpenAutoView(QWidget *parent) : QWidget(parent)
{
  MainWindow  *window = qobject_cast<MainWindow *>(parent);

  connect(window, &MainWindow::set_open_auto_state, [this](unsigned int alpha) {
    if(this->worker != nullptr) this->worker->set_opacity(alpha);
    if(alpha > 0) this->setFocus();
  });

  QStackedLayout *layout = new QStackedLayout(this);
  layout->setContentsMargins(0,0,0,0);
  layout->addWidget(this->msgWidget());
  #ifdef USE_OMX
    QWidget *omx_backdrop = new QWidget(this);
    omx_backdrop->setStyleSheet("background-color: black;");
    layout->addWidget(omx_backdrop);
  #endif
}

void OpenAutoView::start_worker()
{
  QStackedLayout *layout = qobject_cast<QStackedLayout *>(this->layout());
  auto callback = [layout](bool is_active) { layout->setCurrentIndex(is_active ? 1 : 0); };

  if(this->worker == nullptr) this->worker = new OpenAutoWorker(callback, this);

  this->worker->start();
}

QWidget *OpenAutoView::msg_widget()
{
  QWidget *widget = new QWidget(this);
  QVBoxLayout *layout = new QVBoxLayout(widget);
  layout->setContentsMargins(24,24,24,24);

  QLabel *top_msg = new QLabel("waiting for device. . .", widget);
  //top_msg->setFont(Theme::font_16);
  top_msg->setAlignment(Qt::AlignHCenter);
  layout->addStretch();
  layout->addWidget(top_msg);

  QLabel *bottom_msg = new QLabel("plug in your device to start OpenAuto", widget);
  //bottom_msg->setFont(Theme::font_16);
  bottom_msg->setAlignment(Qt::AlignHCenter);
  layout->addWidget(bottom_msg);
  layout->addStretch();

  return widget;
}
    
