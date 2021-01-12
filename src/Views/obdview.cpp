/*
 * obdview.cpp
 * Implementation of the obdview
 * 
 */


#include <Views/obdview.hpp>

#include <App/config.hpp>
#include <App/theme.hpp>
//#include <App/window.hpp>

void OBDFrame::mouseDoubleClickEvent(QMouseEvent *)
{
    ToggleFullscreen();
    emit doubleClicked(fullscreen);
}

OBDView::Settings::Settings(QWidget *parent) : QWidget(parent)
{
    bluetooth = Bluetooth::getInstance();
    theme = Theme::getInstance();
    config = Config::getInstance();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(6, 0, 6, 0);

    layout->addWidget(settingsWidget());
}

QWidget *OBDView::Settings::settingsWidget()
{
    QWidget *widget = new QWidget(this);
    //QVBoxLayout *layout = new QVBoxLayout(widget);
/*
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
*/
    QScrollArea *scroll_area = new QScrollArea(this);
    Theme::ToTouchScroller(scroll_area);
    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(widget);

    return scroll_area;
}



OBDView::OBDView(QWidget *parent) : QStackedWidget(parent)
{
    config = Config::getInstance();
    theme = Theme::getInstance();

    frame = new OBDFrame(this);

    std::function<void(bool)> callback = [frame = frame](bool active) { frame->toggle(active); };
//    worker = new Worker(callback, theme->Mode, frame);

    connect(frame, &OBDFrame::toggle, [this](bool enable) {
        if (!enable && frame->IsFullscreen()) {
            addWidget(frame);
            frame->ToggleFullscreen();
        }
        setCurrentIndex(enable ? 1 : 0);
    });
    connect(frame, &OBDFrame::doubleClicked, [this](bool fullscreen) {
        if (fullscreen) {
            emit toggleFullscreen(frame);
        }
        else {
            addWidget(frame);
            setCurrentWidget(frame);
        }
//        worker->UpdateSize();
    });
//    connect(theme, &Theme::modeUpdated, [this](bool mode) { worker->SetNightMode(mode); });

    addWidget(connectMsg());
    addWidget(frame);
}

void OBDView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    frame->resize(size());
//    worker->UpdateSize();
}

QWidget *OBDView::connectMsg()
{
    QWidget *widget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);

    QLabel *label = new QLabel("connect device to start OBD", widget);
    label->setAlignment(Qt::AlignCenter);

    QHBoxLayout *layout2 = new QHBoxLayout();
    layout2->setContentsMargins(0, 0, 0, 0);
    layout2->setSpacing(0);
    layout->addStretch();
    layout->addWidget(label);
    layout->addStretch();

    return widget;
}
