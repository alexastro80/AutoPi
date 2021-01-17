#include <App/View.hpp>

View::View(QWidget *parent, Worker* _worker) : QStackedWidget(parent)
{
    config = Config::getInstance();
    theme = Theme::getInstance();

    frame = new Frame(this);
    worker = _worker;
    //std::function<void(bool)> callback = [frame = frame](bool active) { frame->toggle(active); };
    

    connect(frame, &Frame::toggle, [this](bool enable) {
        if (!enable && frame->IsFullscreen()) {
            addWidget(frame);
            frame->ToggleFullscreen();
        }
        setCurrentIndex(enable ? 1 : 0);
    });
    connect(frame, &Frame::doubleClicked, [this](bool fullscreen) {
        if (fullscreen) {
            emit toggleFullscreen(frame);
        }
        else {
            addWidget(frame);
            setCurrentWidget(frame);
        }
        
        if(worker != nullptr) worker->UpdateSize();
    });
    if(worker != nullptr) connect(theme, &Theme::modeUpdated, [this](bool mode) { worker->SetNightMode(mode); });
    QWidget* displayWidget = nullptr;
    if(worker != nullptr) displayWidget = worker->Display();
    if(displayWidget != nullptr) addWidget(displayWidget);
    if(frame != nullptr) addWidget(frame);
}

void View::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if(frame != nullptr) frame->resize(size());
    if(worker != nullptr) worker->UpdateSize();
}
