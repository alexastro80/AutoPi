#ifndef OBD_VIEW_HPP
#define OBD_VIEW_HPP

#include <QtWidgets>
#include <thread>
#include "App/config.hpp"
#include "App/bluetooth.hpp"
#include "App/theme.hpp"

class OBDFrame : public QWidget 
{
	Q_OBJECT
public:
    OBDFrame(QWidget *parent) : QWidget(parent) {}

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


class OBDView : public QStackedWidget
{
    Q_OBJECT
public:
    OBDView(QWidget *parent = nullptr);

//    inline void PassKeyEvent(QKeyEvent *event) { worker->SendKeyEvent(event); }

   protected:
    void resizeEvent(QResizeEvent *event);

   private:
    class Settings : public QWidget {
       public:
        Settings(QWidget *parent = nullptr);

       private:
        QWidget *settingsWidget();
//        QBoxLayout *rhdRowWidget();
//        QBoxLayout *frameRateRowWidget();
//        QBoxLayout *resolutionRowWidget();
//        QBoxLayout *dpiRowWidget();
//        QBoxLayout *dpiWidget();
//        QBoxLayout *rtAudioRowwidget();
//        QBoxLayout *audioChannelsRowWidget();
//        QBoxLayout *bluetoothRowWidget();
//        QBoxLayout *touchscreenRowWidget();
//        QCheckBox *buttonCheckbox(QString name, QString key, aasdk::proto::enums::ButtonCode::Enum code);
//        QBoxLayout *buttonsRowWidget();

        Bluetooth *bluetooth;
        Config *config;
        Theme *theme;
    };

    QWidget *connectMsg();

    Config *config;
    Theme *theme;
    OBDFrame *frame;
//    OpenAutoWorker *worker;

   signals:
    void toggleFullscreen(QWidget *widget);

};
#endif
