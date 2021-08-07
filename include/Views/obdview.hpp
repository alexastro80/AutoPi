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

#include <vector>
#include <string.h>
#include <filesystem>
#include <dirent.h>

#define RPM_CODE         0x0C
#define SPEED_CODE       0x0D
#define RUN_TIME_CODE    0x1F
#define FUEL_RATE_CODE   0x5E
#define FUEL_LEVEL_CODE  0x2F
#define ODO_CODE         0x21     

#define NUM_WIDGETS 4
#define NUM_CODES	256

class OBDWorker: public Worker {
Q_OBJECT
public:
	OBDWorker(QWidget *_frame);
	~OBDWorker();
	void UpdateSize();
	QWidget* Display() override;
	void Paint(QPainter &painter) override {
		if (centerGauge != nullptr)
			centerGauge->Paint(painter);
		if (leftGauge != nullptr)
			leftGauge->Paint(painter);
		if (rightGauge != nullptr)
			rightGauge->Paint(painter);
	}

	static std::vector<std::string> getOBDOptions() {
		std::cout << "Getting Bluetooth options...\n";
		system("./lib/scripts/getdevices.sh");
		std::ifstream file("lib/scripts/bluetoothDevices.txt");
		std::vector<std::string> options;
		std::string line = "";
		while (std::getline(file, line)) {
			std::cout << "Address: " << line <<"\n";
			options.push_back(line);
		}
		file.close();

		return options;
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
		max = NUM_WIDGETS
	};

	std::string obdAddress = "";
	bool connected = false;
	bool enabled = false;
public slots:
	void update();
	void recvDatagram();
	void setOBD(std::string nameAndAddress) {
		int start = nameAndAddress.find("{");
		int end = nameAndAddress.find("}");
		if (start >= 0 && end > 0) {
			obdAddress = nameAndAddress.substr(start + 1, end - start - 2);
			connectOBD();
		}
	}
	void enableOBD(bool enable) {
		enabled = enable;
		if (enable && !connected && !enabled) {
			connected = connectOBD();
			if (connected)
				std::cout << "Connected\n";
		}

	}
	bool connectOBD() {
		std::cout << "\tConnecting OBD. . .\n";
		if (connected) {
			system("reboot");
		} else if (!enabled) {
			std::cout << "\tNot Enabled!\n";
		} else {
			if (obdAddress != "") {
				std::string command = "obd.sh "
					+ obdAddress + " &";
				system(command.c_str());
				connected = true;
			} else {
				std::cout <<"\tNo address\n";
			}
		}
		return false;
	}

private:
	void getValue(char code);
	void setCodeWidgetRelationship(int widget, char code);

	QUdpSocket *mySocket = nullptr;
	std::vector<QByteArray> packets;

	char widgetMap[NUM_WIDGETS];
	int codeMap[NUM_CODES];

	QWidget *parent = nullptr;
	QTimer *updateTimer = nullptr;
	QLabel *topIndicator = nullptr;
	QLabel *bottomIndicator1 = nullptr;
	QLabel *bottomIndicator2 = nullptr;

	Gauge *centerGauge = nullptr;
	Gauge *leftGauge = nullptr;
	Gauge *rightGauge = nullptr;
	CenterIndicator *centerIndicator = nullptr;

	const int myPort = 2223;
	const int serverPort = 2222;
};

#endif
