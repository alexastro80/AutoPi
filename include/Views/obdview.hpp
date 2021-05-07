#ifndef OBD_VIEW_HPP
#define OBD_VIEW_HPP

#include <thread>
#include <QtWidgets>
#include <QUdpSocket>
#include <QNetworkDatagram>

#include <App/config.hpp>
#include <App/bluetooth.hpp>
#include <App/theme.hpp>
#include <App/Worker.hpp>
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


class OBDWorker : public Worker
{
    Q_OBJECT
public:
    OBDWorker(QWidget* _frame);
    ~OBDWorker();
    void UpdateSize();
    QWidget* Display() override;
    void Paint(QPainter& painter) override {
    	if(centerGauge != nullptr) centerGauge->Paint(painter);
    	if(leftGauge != nullptr) leftGauge->Paint(painter);
    	if(rightGauge != nullptr) rightGauge->Paint(painter);
    }
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
    Gauge* rightGauge = nullptr;
    CenterIndicator* centerIndicator = nullptr;

    const int myPort = 2223;
    const int serverPort = 2222;
};


#endif
