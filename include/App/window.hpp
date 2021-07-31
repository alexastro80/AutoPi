#pragma once

#include <stdlib.h>

#include <QBluetoothDeviceInfo>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothServiceInfo>
#include <QMainWindow>
#include <QStringList>
#include <QtDBus/QtDBus>
#include <QtWidgets>

#include "App/config.hpp"
#include "App/shortcuts.hpp"
#include "Views/openauto.hpp"
#include <Views/obdview.hpp>
#include "App/theme.hpp"
#include <App/View.hpp>
#include <App/SettingsManager.hpp>

class MainWindow: public QMainWindow {
Q_OBJECT

public:
	MainWindow();
	void AddWidget(QWidget *widget);

	inline QList<QAbstractButton*> Pages() {
		return railGroup->buttons();
	}
	bool startupEnabled = false;
	bool setStartup = false;
	bool setFlip = false;
	bool setJoystick = false;
public slots:
	void startup(bool enable) {
		if (enable != setStartup) {
			if (enable) {
				system("./lib/scripts/Startup/AddToStartup.sh &");
			} else {
				system("./lib/scripts/Startup/RemoveFromStartup.sh");
			}
			setStartup = enable;
		}
	}
	void flip(bool enable) {
		if (setFlip != enable) {
			if (enable) {
				system("./lib/scripts/Startup/flip.sh &");
			} else
				system("./lib/scripts/Startup/unflip.sh &");
			setFlip = enable;
		}

	}
	void joystick(bool enable) {
		if (setJoystick != enable) {
			if (enable) {
				system("./lib/scripts/Startup/enableJoystick.sh &");
			} else
				system("./lib/scripts/Startup/disableJoystick.sh");
			setJoystick = enable;
		}
	}


protected:
	void showEvent(QShowEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

private:
	Config *config;
	Theme *theme;
	Shortcuts *shortcuts;

	View *openauto;
	View *obdview;
	View *settings;

	OpenAuto *openautoWorker;
	OBDWorker *obdWorker;
	SettingsManager *settingsManager;

	QStackedWidget *stack;
	QVBoxLayout *rail;
	QButtonGroup *railGroup;
	QStackedLayout *pages;
	QHBoxLayout *bar;

	void initConfig();
	void initTheme();
	void initUI();
	void initShortcuts();
	void initSettings();
	QLayout* body();
	void addPages();
	void addPage(QString name, QWidget *page, QString icon);

	QWidget* controlsBar();
	QLayout* quickViews();
	QWidget* brightnessWidget(bool skip_buttons = false);
	QWidget* controlsWidget();
	QWidget* powerControl();
	QWidget* saveControl();
};
