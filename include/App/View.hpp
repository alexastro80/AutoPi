#ifndef VIEW_HPP
#define VIEW_HPP
#include <QtWidgets>
#include <App/theme.hpp>
#include <App/config.hpp>
#include "Frame.hpp"
#include "Worker.hpp"
class View : public QStackedWidget
{
Q_OBJECT

public:
    View(QWidget *parent = nullptr, Worker* _worker = nullptr);
    inline Frame* GetFrame() { return frame; }
    inline void PassKeyEvent(QKeyEvent *event) { worker->SendKeyEvent(event); }
    inline void SetWorker(Worker* _worker) { worker = _worker; }
protected:
    void paintEvent(QPaintEvent * event) { if(frame != nullptr) frame->update(); }
    void resizeEvent(QResizeEvent *event);
private:

//    QWidget *connectMsg();

    Config *config;
    Theme *theme;
    Frame *frame;
    Worker *worker;

signals:
    void toggleFullscreen(QWidget *widget);
};
#endif
