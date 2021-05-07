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

OBDWorker::OBDWorker(QWidget* _parent) : Worker()
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
    QHBoxLayout* obdLayout = new QHBoxLayout();
    obdLayout->setAlignment(Qt::AlignCenter);
    if(parent != nullptr) parent->setLayout(obdLayout);

    QWidget* centerWidget = new QWidget(parent);
    QVBoxLayout* centerLayout = new QVBoxLayout();
    centerWidget->setLayout(centerLayout);
    obdLayout->addWidget(centerWidget);

    topIndicator = new QLabel("00:00:00");
    topIndicator->setAlignment(Qt::AlignCenter);
    centerLayout->addWidget(topIndicator);
/*
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
*/
    //displayLayout->addWidget(centerGauge);
    //displayLayout->addWidget(leftGauge);
    centerLayout->addSpacerItem(new QSpacerItem(1,70));
    centerIndicator = new CenterIndicator(parent);
    centerLayout->addWidget(centerIndicator);
    centerLayout->addSpacerItem(new QSpacerItem(1,20));
    bottomIndicator1 = new QLabel("001340.0");
    bottomIndicator1->setAlignment(Qt::AlignCenter);
    centerLayout->addWidget(bottomIndicator1);
    
    bottomIndicator2 = new QLabel("32.6 MPG");
    bottomIndicator2->setAlignment(Qt::AlignCenter);
    centerLayout->addWidget(bottomIndicator2);
    centerLayout->setContentsMargins(0, 200, 0, 250);


    QPoint centerPoint = QGuiApplication::primaryScreen()->geometry().center();
    int xCenter = centerPoint.x()+2;
    int yCenter = centerPoint.y()-5;
    std::cout << "X: " << xCenter;
    std::cout << "\nY:" << yCenter;

    centerGauge = new Gauge(parent,xCenter,yCenter);
    centerGauge->RadiusOuter = 125;
    centerGauge->RadiusInner = centerGauge->RadiusOuter - 40;
    centerGauge->Max = 100;
    centerGauge->DrawAll = false;
    centerGauge->ThetaStart = 120;
    centerGauge->ThetaEnd = 420;
    centerGauge->SetValue(50);

    leftGauge = new Gauge(parent,xCenter,yCenter);
	leftGauge->RadiusInner = centerGauge->RadiusOuter + 10;
	leftGauge->RadiusOuter = leftGauge->RadiusInner + 40;
	leftGauge->Max = 100;
	leftGauge->DrawAll = false;
	leftGauge->ThetaStart = 120;
	leftGauge->ThetaEnd = 240;
	leftGauge->SetValue(50);

	rightGauge = new Gauge(parent,xCenter,yCenter);
	rightGauge->RadiusInner = centerGauge->RadiusOuter + 10;
	rightGauge->RadiusOuter = leftGauge->RadiusInner + 40;
	rightGauge->Max = 100;
	rightGauge->DrawAll = false;
	rightGauge->ThetaStart = 60;
	rightGauge->ThetaEnd = -60;
	rightGauge->SetValue(50);

    if(parent != nullptr) parent->update();


    //displayLayout->addStretch();

}
void OBDWorker::UpdateSize()
{
    if(centerIndicator != nullptr)
    {
        centerIndicator->SetValue(0.0);
        leftGauge->SetValue(leftGauge->Min);
        centerGauge->SetValue(centerGauge->Min);
        if(parent != nullptr) parent->update();
    }
}
QWidget* OBDWorker::Display()
{
   
    return parent;
    
}
OBDWorker::~OBDWorker()
{
    if(mySocket != nullptr) delete mySocket;

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
    if(parent != nullptr) parent->update();
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
