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
#include <Widgets/Indicator.hpp>
#include <Widgets/Gauge.hpp>
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
    for(int i = 0; i < NUM_CODES; i++) codeMap[i] = -1;
    //Setup Widget Code Relationship (1:1)
    setCodeWidgetRelationship(WidgetEnum::TopLabel, RUN_TIME_CODE);
    setCodeWidgetRelationship(WidgetEnum::CenterLabel, SPEED_CODE);
    setCodeWidgetRelationship(WidgetEnum::BottomLabel1, FUEL_RATE_CODE);
    setCodeWidgetRelationship(WidgetEnum::BottomLabel2, ODO_CODE);
    
    for(int i = 0; i < NUM_WIDGETS; i++) std::cout << widgetMap[i] << " ";

    std::cout << "\n";

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

    centerGauge = new Gauge(this, 500,250);
    centerGauge->RadiusOuter = 100;
    centerGauge->RadiusInner = 75;
    centerGauge->ThetaStart = 135;
    centerGauge->ThetaEnd = 405;
    centerGauge->Max = 8000;
    centerGauge->Min = 0;
    centerGauge->ForegroundColor = Qt::red;

    leftGauge = new Gauge(this, 500, 250);
    leftGauge->RadiusInner = 105;
    leftGauge->RadiusOuter = 140;
    leftGauge->ThetaStart = 135;
    leftGauge->ThetaEnd = 225;
    leftGauge->Max = 40;
    leftGauge->Min = 0;
    leftGauge->ForegroundColor = Qt::green;

    displayLayout->addWidget(centerGauge);
    displayLayout->addWidget(leftGauge);

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
        centerIndicator->SetValue(0.0);
        leftGauge->SetValue(leftGauge->Min);
        centerGauge->SetValue(centerGauge->Min);
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
    if(centerGauge != nullptr) delete centerGauge;
    if(leftGauge != nullptr) delete leftGauge;
}

void OBDWorker::recvDatagram()
{
    if (mySocket != nullptr )
    {
        QByteArray buffer;
        buffer.resize(mySocket->pendingDatagramSize());
        mySocket->readDatagram(buffer.data(), buffer.size());
//        std::cout << "Buffer size: " << buffer.size() << "\n";
        packets.push_back(buffer);
//        std::cout << buffer.toStdString();
    }
}

void OBDWorker::update()
{
    if (packets.size() > 0)
    {
        //Update the GUI
        for (auto packet : packets)
        {
        	if (packet.size() < 11) continue;
        	std::cout << "Processing Packet:\n";
        	char* bytes = packet.data();
        	char data_type = *bytes++;
        	std::cout << "Data Type: " << (int) data_type << "\n";
            if(data_type == 0x01)
            {//Value packet
            	std::cout << "Value Packet\n";

				char valueType = *bytes++;
				int bigEndian = 0;

				char valueMem[8];
				if (bigEndian == 1)
					for (int i = 13; i >= 6; i--) valueMem[13-i] = bytes[i];
				else
					for (int i = 6; i < 14; i++) valueMem[i-6] = bytes[i];

				//for (int i = 0; i < 8; i++) std::cout << (int) valueMem[i] << ",";
				double value = 0;
				memcpy(&value, valueMem, sizeof (value));
				std::cout << "\nValue: " << value << "\n";
				int widgetIndex = codeMap[valueType];
				switch (widgetIndex)
				{
				case WidgetEnum::BottomLabel1:
					bottomIndicator1->setText(QString::number(value, 'f', 1));
					break;
				case WidgetEnum::BottomLabel2:
					bottomIndicator2->setText(QString::number(value, 'f', 1));
					break;
				case WidgetEnum::CenterLabel:
					centerIndicator->SetCenterLabel(QString::number(value, 'f', 2));
					break;
				case WidgetEnum::TopLabel:
					topIndicator->setText(QString::number(value, 'f', 1));
					break;
				}
            }
            else if (data_type == 0x03)
            {//Status packet
                
            }

        }
        packets.clear();
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
    int CODE_LEN = 1;
    char packet[] = {2,code};
    QByteArray message(packet);
    mySocket->writeDatagram(message, QHostAddress::LocalHost, serverPort);
}

inline void OBDWorker::setCodeWidgetRelationship(int widget, char code)
{
	if ((widget < NUM_WIDGETS && widget > -1)
			&& code < NUM_CODES)
	{
		widgetMap[widget] = code;
		codeMap[code] = widget;
	}
}
