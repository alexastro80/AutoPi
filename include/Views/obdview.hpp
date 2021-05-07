#ifndef OBD_VIEW_HPP
#define OBD_VIEW_HPP

#include <thread>
#include <QtWidgets>
#include <QUdpSocket>
#include <QNetworkDatagram>

#include <App/config.hpp>
#include <App/bluetooth.hpp>
#include <App/theme.hpp>
#include <Widgets/CenterIndicator.hpp>
#include <Widgets/Gauge.hpp>
#define RPM_CODE         0x0C
#define SPEED_CODE       0x0D
#define RUN_TIME_CODE    0x1F
#define FUEL_RATE_CODE   0x5E
#define FUEL_LEVEL_CODE  0x2F
#define ODO_CODE         0x21     

#define NUM_WIDGETS 4
#define NUM_CODES	256



class OBDWorker;

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

    Config *config = nullptr;
    Theme *theme = nullptr;
    OBDFrame *frame = nullptr;
    OBDWorker *worker = nullptr;

   signals:
    void toggleFullscreen(QWidget *widget);

};


class OBDWorker : public QObject
{
    Q_OBJECT
public:
    OBDWorker(QWidget* _frame);
    ~OBDWorker();
    void UpdateSize();
    QWidget* Display();
//    OBDData RequestData();

    enum WidgetEnum {
        CenterLabel = 0,
        TopLabel,
        BottomLabel1,
        BottomLabel2,
        CenterGauge,
        LeftGauge,
        RightGauge,
        LeftLabels,
        RightLabels = 16,
        max = NUM_WIDGETS};

public slots:
    void update();
    void recvDatagram();
    
private:
    void getValue(char code);
    void setCodeWidgetRelationship(int widget, char code);

    QUdpSocket* mySocket  = nullptr;
    std::vector<QByteArray> packets;
    
    char widgetMap[NUM_WIDGETS];
    int codeMap[NUM_CODES];



    QWidget* parent = nullptr;
    QTimer* updateTimer = nullptr;
    QLabel* topIndicator = nullptr;
    QLabel* bottomIndicator1 = nullptr;
    QLabel* bottomIndicator2 = nullptr;

    Gauge* centerGauge = nullptr;
    Gauge* leftGauge = nullptr;
    CenterIndicator* centerIndicator = nullptr;

    const int myPort = 2223;
    const int serverPort = 2222;
};


#endif
