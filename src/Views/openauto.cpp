#include "Views/openauto.hpp"

#include "App/config.hpp"
#include "App/theme.hpp"
//#include "App/widgets/ip_input.hpp"
//#include "App/widgets/progress.hpp"
#include "App/window.hpp"

OpenAutoWorker::OpenAutoWorker(std::function<void(bool)> callback, bool night_mode, QWidget *frame)
    : QObject(qApp),
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

OpenAutoWorker::~OpenAutoWorker()
{
    std::for_each(threadPool.begin(), threadPool.end(),
                  std::bind(&std::thread::join, std::placeholders::_1));
    libusb_exit(usb_context);
}

void OpenAutoWorker::createUsbWorkers()
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

void OpenAutoWorker::createIOServiceWorkers()
{
    std::function<void()> worker = [this]() { io_service.run(); };

    threadPool.emplace_back(worker);
    threadPool.emplace_back(worker);
    threadPool.emplace_back(worker);
    threadPool.emplace_back(worker);
}

void OpenAutoFrame::mouseDoubleClickEvent(QMouseEvent *)
{
    ToggleFullscreen();
    emit doubleClicked(fullscreen);
}

OpenAutoView::Settings::Settings(QWidget *parent) : QWidget(parent)
{
    bluetooth = Bluetooth::getInstance();
    theme = Theme::getInstance();
    config = Config::getInstance();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(6, 0, 6, 0);

    layout->addWidget(settingsWidget());
}

QWidget *OpenAutoView::Settings::settingsWidget()
{
    QWidget *widget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(widget);

    layout->addLayout(rhdRowWidget(), 1);
    layout->addWidget(Theme::Br(), 1);
    layout->addLayout(frameRateRowWidget(), 1);
    layout->addLayout(resolutionRowWidget(), 1);
    layout->addLayout(dpiRowWidget(), 1);
    layout->addWidget(Theme::Br(), 1);
    layout->addLayout(rtAudioRowwidget(), 1);
    layout->addLayout(audioChannelsRowWidget(), 1);
    layout->addWidget(Theme::Br(), 1);
    layout->addLayout(bluetoothRowWidget(), 1);
    layout->addWidget(Theme::Br(), 1);
    layout->addLayout(touchscreenRowWidget(), 1);
    layout->addLayout(buttonsRowWidget(), 1);

    QScrollArea *scroll_area = new QScrollArea(this);
    Theme::ToTouchScroller(scroll_area);
    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(widget);

    return scroll_area;
}

QBoxLayout *OpenAutoView::Settings::rhdRowWidget()
{
    QHBoxLayout *layout = new QHBoxLayout();

    QLabel *label = new QLabel("Right-Hand-Drive");
    layout->addWidget(label, 1);
/*
    Switch *toggle = new Switch();
    toggle->scale(config->Scale);
    toggle->setChecked(config->openautoConfig->getHandednessOfTrafficType() ==
                       openauto::configuration::HandednessOfTrafficType::RIGHT_HAND_DRIVE);
    connect(config, &Config::scaleChanged, [toggle](double scale) { toggle->scale(scale); });
    connect(toggle, &Switch::stateChanged, [config = config](bool state) {
        config->openautoConfig->setHandednessOfTrafficType(
            state ? openauto::configuration::HandednessOfTrafficType::RIGHT_HAND_DRIVE
                  : openauto::configuration::HandednessOfTrafficType::LEFT_HAND_DRIVE);
    });
    layout->addWidget(toggle, 1, Qt::AlignHCenter);
*/
    return layout;
}

QBoxLayout *OpenAutoView::Settings::frameRateRowWidget()
{
    QHBoxLayout *layout = new QHBoxLayout();

    QLabel *label = new QLabel("Frame Rate");
    layout->addWidget(label, 1);

    QGroupBox *group = new QGroupBox();
    QVBoxLayout *group_layout = new QVBoxLayout(group);

    QRadioButton *fps_30_button = new QRadioButton("30fps", group);
    fps_30_button->setChecked(config->openautoConfig->getVideoFPS() == aasdk::proto::enums::VideoFPS::_30);
    connect(fps_30_button, &QRadioButton::clicked,
            [config = config]() { config->openautoConfig->setVideoFPS(aasdk::proto::enums::VideoFPS::_30); });
    group_layout->addWidget(fps_30_button);

    QRadioButton *fps_60_button = new QRadioButton("60fps", group);
    fps_60_button->setChecked(config->openautoConfig->getVideoFPS() == aasdk::proto::enums::VideoFPS::_60);
    connect(fps_60_button, &QRadioButton::clicked,
            [config = config]() { config->openautoConfig->setVideoFPS(aasdk::proto::enums::VideoFPS::_60); });
    group_layout->addWidget(fps_60_button);

    layout->addWidget(group, 1, Qt::AlignHCenter);

    return layout;
}

QBoxLayout *OpenAutoView::Settings::resolutionRowWidget()
{
    QHBoxLayout *layout = new QHBoxLayout();

    QLabel *label = new QLabel("Resolution");
    layout->addWidget(label, 1);

    QGroupBox *group = new QGroupBox();
    QVBoxLayout *group_layout = new QVBoxLayout(group);

    QRadioButton *res_480_button = new QRadioButton("480p", group);
    res_480_button->setChecked(config->openautoConfig->getVideoResolution() ==
                               aasdk::proto::enums::VideoResolution::_480p);
    connect(res_480_button, &QRadioButton::clicked, [config = config]() {
        config->openautoConfig->setVideoResolution(aasdk::proto::enums::VideoResolution::_480p);
    });
    group_layout->addWidget(res_480_button);

    QRadioButton *res_720_button = new QRadioButton("720p", group);
    res_720_button->setChecked(config->openautoConfig->getVideoResolution() ==
                               aasdk::proto::enums::VideoResolution::_720p);
    connect(res_720_button, &QRadioButton::clicked, [config = config]() {
        config->openautoConfig->setVideoResolution(aasdk::proto::enums::VideoResolution::_720p);
    });
    group_layout->addWidget(res_720_button);

    QRadioButton *res_1080_button = new QRadioButton("1080p", group);
    res_1080_button->setChecked(config->openautoConfig->getVideoResolution() ==
                                aasdk::proto::enums::VideoResolution::_1080p);
    connect(res_1080_button, &QRadioButton::clicked, [config = config]() {
        config->openautoConfig->setVideoResolution(aasdk::proto::enums::VideoResolution::_1080p);
    });
    group_layout->addWidget(res_1080_button);

    layout->addWidget(group, 1, Qt::AlignHCenter);

    return layout;
}

QBoxLayout *OpenAutoView::Settings::dpiRowWidget()
{
    QHBoxLayout *layout = new QHBoxLayout();

    QLabel *label = new QLabel("DPI");
    layout->addWidget(label, 1);

    layout->addLayout(dpiWidget(), 1);

    return layout;
}

QBoxLayout *OpenAutoView::Settings::dpiWidget()
{
    QHBoxLayout *layout = new QHBoxLayout();

    QSlider *slider = new QSlider(Qt::Orientation::Horizontal);
    slider->setTracking(false);
    slider->setRange(0, 512);
    slider->setValue(config->openautoConfig->getScreenDPI());
    QLabel *value = new QLabel(QString::number(slider->value()));
    connect(slider, &QSlider::valueChanged, [config = config, value](int position) {
        config->openautoConfig->setScreenDPI(position);
        value->setText(QString::number(position));
    });
    connect(slider, &QSlider::sliderMoved, [value](int position) {
        value->setText(QString::number(position));
    });

    layout->addStretch(2);
    layout->addWidget(slider, 4);
    layout->addWidget(value, 2);

    return layout;
}

QBoxLayout *OpenAutoView::Settings::rtAudioRowwidget()
{
    QHBoxLayout *layout = new QHBoxLayout();

    QLabel *label = new QLabel("RtAudio");
    layout->addWidget(label, 1);
/*
    Switch *toggle = new Switch();
    toggle->scale(config->Scale);
    toggle->setChecked(config->openautoConfig->getAudioOutputBackendType() ==
                       openauto::configuration::AudioOutputBackendType::RTAUDIO);
    connect(config, &Config::scaleChanged, [toggle](double scale) { toggle->scale(scale); });
    connect(toggle, &Switch::stateChanged, [config = config](bool state) {
        config->openautoConfig->setAudioOutputBackendType(state
                                                               ? openauto::configuration::AudioOutputBackendType::RTAUDIO
                                                               : openauto::configuration::AudioOutputBackendType::QT);
    });
    layout->addWidget(toggle, 1, Qt::AlignHCenter);
*/
    return layout;
}

QBoxLayout *OpenAutoView::Settings::audioChannelsRowWidget()
{
    QHBoxLayout *layout = new QHBoxLayout();

    QLabel *label = new QLabel("Audio Channels");
    layout->addWidget(label, 1);

    QGroupBox *group = new QGroupBox();
    QVBoxLayout *group_layout = new QVBoxLayout(group);

    QCheckBox *music_button = new QCheckBox("Music", group);
    music_button->setChecked(config->openautoConfig->musicAudioChannelEnabled());
    connect(music_button, &QCheckBox::toggled,
            [config = config](bool checked) { config->openautoConfig->setMusicAudioChannelEnabled(checked); });
    group_layout->addWidget(music_button);
    group_layout->addStretch(2);

    QCheckBox *speech_button = new QCheckBox("Speech", group);
    speech_button->setChecked(config->openautoConfig->speechAudioChannelEnabled());
    connect(speech_button, &QCheckBox::toggled,
            [config = config](bool checked) { config->openautoConfig->setSpeechAudioChannelEnabled(checked); });
    group_layout->addWidget(speech_button);

    layout->addWidget(group, 1, Qt::AlignHCenter);

    return layout;
}

QBoxLayout *OpenAutoView::Settings::bluetoothRowWidget()
{
    QHBoxLayout *layout = new QHBoxLayout();

    QLabel *label = new QLabel("Bluetooth");
    layout->addWidget(label, 1);
/*
    Switch *toggle = new Switch();
    toggle->scale(config->Scale);
    toggle->setChecked(config->openautoConfig->getBluetoothAdapterType() ==
                       openauto::configuration::BluetoothAdapterType::LOCAL);
    connect(config, &Config::scaleChanged, [toggle](double scale) { toggle->scale(scale); });
    connect(toggle, &Switch::stateChanged, [config = config](bool state) {
        config->openautoConfig->setBluetoothAdapterType(state ? openauto::configuration::BluetoothAdapterType::LOCAL
                                                               : openauto::configuration::BluetoothAdapterType::NONE);
    });
    layout->addWidget(toggle, 1, Qt::AlignHCenter);
*/
    return layout;
}

QBoxLayout *OpenAutoView::Settings::touchscreenRowWidget()
{
    QHBoxLayout *layout = new QHBoxLayout();

    QLabel *label = new QLabel("Touchscreen");
    layout->addWidget(label, 1);
/*
    Switch *toggle = new Switch();
    toggle->scale(config->Scale);
    toggle->setChecked(config->openautoConfig->getTouchscreenEnabled());
    connect(config, &Config::scaleChanged, [toggle](double scale) { toggle->scale(scale); });
    connect(toggle, &Switch::stateChanged,
            [config = config](bool state) { config->openautoConfig->setTouchscreenEnabled(state); });
    layout->addWidget(toggle, 1, Qt::AlignHCenter);
*/
    return layout;
}

QCheckBox *OpenAutoView::Settings::buttonCheckbox(QString name, QString key,
                                                   aasdk::proto::enums::ButtonCode::Enum code)
{
    QCheckBox *checkbox = new QCheckBox(QString("%1 [%2]").arg(name).arg(key));
    checkbox->setChecked(std::find(config->openautoButtonCodes.begin(),
                                   config->openautoButtonCodes.end(),
                                   code) != config->openautoButtonCodes.end());
    connect(checkbox, &QCheckBox::toggled, [config = config, code](bool checked) {
        if (checked) {
            config->openautoButtonCodes.push_back(code);
        }
        else {
            config->openautoButtonCodes.erase(
                std::remove(config->openautoButtonCodes.begin(), config->openautoButtonCodes.end(), code),
                config->openautoButtonCodes.end());
        }
    });

    return checkbox;
}

QBoxLayout *OpenAutoView::Settings::buttonsRowWidget()
{
    QHBoxLayout *layout = new QHBoxLayout();

    QLabel *label = new QLabel("Buttons");

    QGroupBox *group = new QGroupBox();
    QVBoxLayout *group_layout = new QVBoxLayout(group);

    group_layout->addWidget(buttonCheckbox("Enter", "Enter", aasdk::proto::enums::ButtonCode::ENTER));
    group_layout->addWidget(buttonCheckbox("Left", "Left", aasdk::proto::enums::ButtonCode::LEFT));
    group_layout->addWidget(buttonCheckbox("Right", "Right", aasdk::proto::enums::ButtonCode::RIGHT));
    group_layout->addWidget(buttonCheckbox("Up", "Up", aasdk::proto::enums::ButtonCode::UP));
    group_layout->addWidget(buttonCheckbox("Down", "Down", aasdk::proto::enums::ButtonCode::DOWN));
    group_layout->addWidget(buttonCheckbox("Back", "Esc", aasdk::proto::enums::ButtonCode::BACK));
    group_layout->addWidget(buttonCheckbox("Home", "H", aasdk::proto::enums::ButtonCode::HOME));
    group_layout->addWidget(buttonCheckbox("Phone", "P", aasdk::proto::enums::ButtonCode::PHONE));
    group_layout->addWidget(buttonCheckbox("Call End", "O", aasdk::proto::enums::ButtonCode::CALL_END));
    group_layout->addWidget(buttonCheckbox("Play", "X", aasdk::proto::enums::ButtonCode::PLAY));
    group_layout->addWidget(buttonCheckbox("Pause", "C", aasdk::proto::enums::ButtonCode::PAUSE));
    group_layout->addWidget(buttonCheckbox("Prev Track", "V", aasdk::proto::enums::ButtonCode::PREV));
    group_layout->addWidget(buttonCheckbox("Next Track", "N", aasdk::proto::enums::ButtonCode::NEXT));
    group_layout->addWidget(buttonCheckbox("Toggle Play", "B", aasdk::proto::enums::ButtonCode::TOGGLE_PLAY));
    group_layout->addWidget(buttonCheckbox("Voice", "M", aasdk::proto::enums::ButtonCode::MICROPHONE_1));
    group_layout->addWidget(buttonCheckbox("Scroll", "1/2", aasdk::proto::enums::ButtonCode::SCROLL_WHEEL));

    layout->addWidget(label, 1);
    layout->addWidget(group, 1, Qt::AlignHCenter);

    return layout;
}

OpenAutoView::OpenAutoView(QWidget *parent) : QStackedWidget(parent)
{
    config = Config::getInstance();
    theme = Theme::getInstance();

    frame = new OpenAutoFrame(this);

    std::function<void(bool)> callback = [frame = frame](bool active) { frame->toggle(active); };
    worker = new OpenAutoWorker(callback, theme->Mode, frame);

    connect(frame, &OpenAutoFrame::toggle, [this](bool enable) {
        if (!enable && frame->IsFullscreen()) {
            addWidget(frame);
            frame->ToggleFullscreen();
        }
        setCurrentIndex(enable ? 1 : 0);
    });
    connect(frame, &OpenAutoFrame::doubleClicked, [this](bool fullscreen) {
        if (fullscreen) {
            emit toggleFullscreen(frame);
        }
        else {
            addWidget(frame);
            setCurrentWidget(frame);
        }
        worker->UpdateSize();
    });
    connect(theme, &Theme::modeUpdated, [this](bool mode) { worker->SetNightMode(mode); });

    addWidget(connectMsg());
    addWidget(frame);
}

void OpenAutoView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    frame->resize(size());
    worker->UpdateSize();
}

QWidget *OpenAutoView::connectMsg()
{
    QWidget *widget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);

    QLabel *label = new QLabel("connect device to start Android Auto", widget);
    label->setAlignment(Qt::AlignCenter);

    QHBoxLayout *layout2 = new QHBoxLayout();
    layout2->setContentsMargins(0, 0, 0, 0);
    layout2->setSpacing(0);
/*
    Dialog *dialog = new Dialog(true, window());
    dialog->set_body(new Settings());
    QPushButton *save_button = new QPushButton("save");
    connect(save_button, &QPushButton::clicked, [this]() {
        config->openautoConfig->setButtonCodes(config->openautoButtonCodes);
        config->openautoConfig->save();
    });
    dialog->set_button(save_button);

    QPushButton *settings_button = new QPushButton(widget);
    settings_button->setFlat(true);
    settings_button->setIconSize(Theme::icon_24);
    settings_button->setIcon(theme->MakeButtonIcon("settings", settings_button));
    connect(settings_button, &QPushButton::clicked, [dialog]() { dialog->open(); });

    layout2->addStretch();
    layout2->addWidget(settings_button);

    layout->addLayout(layout2);
*/    layout->addStretch();
    layout->addWidget(label);
    layout->addStretch();

    return widget;
}
