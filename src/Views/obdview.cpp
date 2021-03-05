/*
 * obdview.cpp
 * Implementation of the obdview
 * 
 */

#include <iostream> //std::fixed
#include <sstream>
#include <iomanip>

#include <QString>

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

    std::cout << "OBD Worker\n";
    worker = new OBDWorker(this);

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
        worker->UpdateSize();
    });
//    connect(theme, &Theme::modeUpdated, [this](bool mode) { worker->SetNightMode(mode); });
    addWidget(worker->Display());
 //   addWidget(connectMsg());
    addWidget(frame);
}

void OBDView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    frame->resize(size());
    worker->UpdateSize();
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

OBDWorker::OBDWorker(QWidget* _parent) : QObject(qApp)
{
    parent = _parent;

    char packet[] = "\0imperial";
    int timeout = 1;
    
    mySocket = new QUdpSocket(this);
    mySocket->bind(QHostAddress::LocalHost, myPort);
    connect(mySocket, SIGNAL(readyRead()), this, SLOT(recvDatagram()));
    
    QNetworkDatagram message(packet,QHostAddress::LocalHost,serverPort);
    mySocket->writeDatagram(message);
    
    for(int i = 0; i < NUM_WIDGETS; i++) widgetMap[i] = 0;
    //Setup WidgetMap
    widgetMap[WidgetEnum::topLabel] = RUN_TIME_CODE;
    widgetMap[WidgetEnum::centerLabel] = SPEED_CODE;
    widgetMap[WidgetEnum::bottomLabel1] = FUEL_RATE_CODE;
    widgetMap[WidgetEnum::bottomLabel2] = ODO_CODE;
    
    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()),this, SLOT(update()));
    updateTimer->start(200);
 
    //UI code
    displayWidget = new QWidget(_parent);
    displayLayout = new QVBoxLayout(displayWidget);
    //QWidget* centerWidget = new QWidget(displayWidget);
    //QVBoxLayout* centerLayout = new QVBoxLayout(centerWidget);
    //centerWidget->setAlignment(Qt::AlignCenter);
    //centerLayout->setAlignment(Qt::AlignCenter);
    //displayLayout->setContentsMargins(30, 30, 30, 30);
    topIndicator = new QLabel("00:00:00");
    topIndicator->setAlignment(Qt::AlignCenter);
    displayLayout->addWidget(topIndicator);
    
    
    centerIndicator = new CenterIndicator(displayWidget);
    displayLayout->addWidget(centerIndicator);
    
    bottomIndicator1 = new QLabel("001340.0");
    bottomIndicator1->setAlignment(Qt::AlignCenter);
    displayLayout->addWidget(bottomIndicator1);
    
    bottomIndicator2 = new QLabel("32.6 MPG");
    bottomIndicator2->setAlignment(Qt::AlignCenter);
    displayLayout->addWidget(bottomIndicator2);
    displayLayout->addStretch();
    
    //displayLayout->addWidget(centerWidget);
    displayLayout->setContentsMargins(30, 30, 30, 30);
    //displayLayout->addStretch();

}
void OBDWorker::UpdateSize()
{
    if(centerIndicator != nullptr)
    {
        centerIndicator->Update(0.0);
    }
}
QWidget* OBDWorker::Display()
{
   
    return displayWidget;
    
}
OBDWorker::~OBDWorker()
{
    if(mySocket != nullptr) delete mySocket;
    if(displayWidget != nullptr) delete displayWidget;
    if(displayLayout != nullptr) delete displayLayout;

    if(parent != nullptr) delete parent;
    if(updateTimer != nullptr) delete updateTimer;
    if(topIndicator != nullptr) delete topIndicator;
    if(bottomIndicator1 != nullptr) delete topIndicator;
    if(bottomIndicator2 != nullptr) delete topIndicator;
    if(centerIndicator != nullptr) delete topIndicator;
    
}

void OBDWorker::recvDatagram()
{
    if (mySocket != nullptr )
    {
        QByteArray buffer;
        buffer.resize(mySocket->pendingDatagramSize());
        mySocket->readDatagram(buffer.data(), buffer.size());
        packets.push_back(buffer);
        std::cout << buffer.toStdString();
    }
}

void OBDWorker::update()
{
    if (packets.size() > 0)
    {
        //Update the GUI
        for (auto packet : packets)
        {
            if(packet.at(0) == (char)1)
            {//Value packet
                for (int i = 0; i < NUM_WIDGETS; i++)
                {
                    if(widgetMap[i] == packet[1])
                    {
                        double* value = (double*)(packet.data()+1);
                        if (value == nullptr) return;
                        switch (i)
                        {
                        case WidgetEnum::bottomLabel1:
                            bottomIndicator1->setText(QString::number(*value, 'f', 1));
                            break;
                        case WidgetEnum::bottomLabel2:
                            bottomIndicator2->setText(QString::number(*value, 'f', 1));
                            break;
                        case WidgetEnum::centerLabel:
                            centerIndicator->SetCenterLabel(QString::number(*value, 'f', 3));
                            break;
                        case WidgetEnum::topLabel:
                            topIndicator->setText(QString::number(*value, 'f', 1));  
                            break; 
                        }    
                    }     
                }      
            }
            else if (packet.at(0) == (char)3)
            {//Status packet
                
            }
            packets.clear();
        }
        
    }
    else 
    {
        for (int i = 0; i < NUM_WIDGETS; i++)
        {
            getValue(widgetMap[i]);
        }
    } 
}

inline void OBDWorker::getValue(char code)
{   
    double x = 0.;
    int CODE_LEN = 1 + sizeof(x);
    char packet[CODE_LEN] = {2,code};
    QByteArray message(packet);
    mySocket->writeDatagram(message, QHostAddress::LocalHost, serverPort);
}
