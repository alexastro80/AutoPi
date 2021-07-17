#include "App/window.hpp"
#include "Value/StringValue.hpp"
#include "Value/StringOption.hpp"
#include "Value/IntValue.hpp"
#include "Value/BoolValue.hpp"

#include <math.h>

#include <QElapsedTimer>
#include <QtWidgets>
#include <cstdlib>
#include <sstream>

MainWindow::MainWindow() {
	//  setAttribute(Qt::WA_TranslucentBackground, true);

	config = Config::getInstance();
	theme = Theme::getInstance();
	shortcuts = Shortcuts::getInstance();

	initTheme();
	initConfig();

	openauto = new View(this);
	Frame *openautoFrame = openauto->GetFrame();
	std::function<void(bool)> callback = [frame = openautoFrame](bool active) {
		frame->toggle(active);
	};
	openautoWorker = new OpenAuto(callback, theme->Mode, openautoFrame);
	openauto->SetWorker(openautoWorker);

	std::cout << "OBD View\n";
	obdview = new View(this);
	Frame *obdFrame = obdview->GetFrame();
	obdWorker = new OBDWorker(obdFrame);
	obdFrame->SetWorker(obdWorker);
	std::cout << "Settings View\n";
	settings = new View(this);


	stack = new QStackedWidget(this);
	rail = new QVBoxLayout();
	railGroup = new QButtonGroup(this);
	pages = new QStackedLayout();
	bar = new QHBoxLayout();

	connect(railGroup, QOverload<int, bool>::of(&QButtonGroup::buttonToggled), // @suppress("Invalid arguments")
	[this](int id, bool) {
		pages->setCurrentIndex(id);
	});
	connect(openauto, &View::toggleFullscreen, [this](QWidget *widget) {
		AddWidget(widget);
	});
	connect(obdview, &View::toggleFullscreen, [this](QWidget *widget) {
		AddWidget(widget);
	});
//    connect(settings, &View::toggleFullscreen, [this](QWidget *widget) { AddWidget(widget); })

	connect(config, &Config::scaleChanged, [theme = theme](double scale) {
		theme->Scale(scale);
	});
	connect(config, &Config::pageChanged,
			[railGroup = railGroup, pages = pages](QWidget *page,
					bool enabled) {
				QAbstractButton *button = railGroup->button(
						pages->indexOf(page));
				button->setVisible(enabled);
			});
	system("sleep 0.1");
	std::cout << "Initializing:\n\tUI\n";
	initUI();
	std::cout << "\tShortcuts\n";
	initShortcuts();
	std::cout << "\tSettings\n";
	initSettings();
}

void MainWindow::AddWidget(QWidget *widget) {
	stack->addWidget(widget);
	stack->setCurrentWidget(widget);
}

void MainWindow::showEvent(QShowEvent *event) {
	QWidget::showEvent(event);

	theme->Update();
	shortcuts->InitShortcuts();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
	QMainWindow::keyPressEvent(event);
	openauto->PassKeyEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
	QMainWindow::keyReleaseEvent(event);
	openauto->PassKeyEvent(event);
}

void MainWindow::initConfig() {
	qApp->setOverrideCursor(
			config->MouseActive ? Qt::ArrowCursor : Qt::BlankCursor);
	config->AddQuickView("none", new QFrame(this));
}

void MainWindow::initTheme() {
	theme->Mode = config->DarkMode;
	theme->Scale(config->Scale);
}

void MainWindow::initUI() {
	QWidget *widget = new QWidget(this);
	QHBoxLayout *layout = new QHBoxLayout(widget);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	rail->setContentsMargins(0, 0, 0, 0);
	rail->setSpacing(0);

	layout->addLayout(rail);
	layout->addLayout(body());

	addPages();

	stack->addWidget(widget);
	setCentralWidget(stack);
}

void MainWindow::initSettings() {
	try {
		std::cout <<"\tGetting Frame\n";
		Frame *settingsFrame = settings->GetFrame();
		std::cout <<"\tNew Settings Manager\n";
		settingsManager = new SettingsManager();
		std::cout <<"\tSetting Worker. . .\n";
		settingsFrame->SetWorker(settingsManager);
		std::cout << "\tAdding General Settings. . .\n";
		settingsManager->Add(new SettingCategory("General"));
		settingsManager->Add(
				new Setting("General", "Theme Mode", theme, SLOT(Update(bool)),
						new BoolValue(true)));
		settingsManager->Add(
				new Setting("General", "Font", new StringValue("Arial")));
		settingsManager->Add(new Setting("General", "Volume", new IntValue(20)));
		std::cout << "\tAdding Startup Settings. . .\n";
		settingsManager->Add(new SettingCategory("Startup"));
		settingsManager->Add(
				new Setting("Startup", "Startup", this, SLOT(startup(bool)),
						new BoolValue(false)));
		settingsManager->Add(
				new Setting("Startup", "Flip", this, SLOT(flip(bool)),
						new BoolValue(false)));
		settingsManager->Add(
				new Setting("Startup", "Joystick Control", this, SLOT(joystick(bool)),
						new BoolValue(false)));

		std::cout << "\tAdding Open Auto Settings. . .\n";
		settingsManager->Add(new SettingCategory("Open Auto"));

		StringOption *frameRates = new StringOption("30");
		frameRates->AddOption("30");
		frameRates->AddOption("60");
		settingsManager->Add(new Setting("Open Auto", "Frame Rate", frameRates));

		StringOption *resolutions = new StringOption("480");
		resolutions->AddOption("480");
		resolutions->AddOption("720");
		resolutions->AddOption("1080");
		settingsManager->Add(
				new Setting("Open Auto", "Resolution", config,
						SLOT(setVideoResolution(std::string)), resolutions));

		settingsManager->Add(new Setting("Open Auto", "DPI", new IntValue(140)));

		std::cout << "\tAdding OBD Settings. . .\n";
		settingsManager->Add(new SettingCategory("OBD"));
		settingsManager->Add(
				new Setting("OBD", "OBD", obdWorker, SLOT(endableOBD(bool)),
						new BoolValue(false)));
		settingsManager->Add(
				new Setting("OBD", "OBD Address", obdWorker,
						SLOT(setOBD(std::string)),
						new StringOption(OBDWorker::getOBDOptions())));
		std::cout<<"\tInitializing Settings. . .\n";
		settingsManager->initialize(settingsFrame);
		std::cout<<"\tLoading Settings. . .\n";
		settingsManager->LoadSettings("AutoPi.csv");
		std::cout<<"\tWriting Settings. . .\n";
		settingsManager->WriteSettings("AutoPi.csv");
	} catch (std::bad_alloc& e) {
		std::cout << "Settings Manager Failed!\nRetrying...\n";
		if (settingsManager != nullptr) {
			delete settingsManager;
			settingsManager=nullptr;
		}
		initSettings();
	}

	std::cout << "Settings Finished\n";

}

void MainWindow::initShortcuts() {
	Shortcut *brightness_down = new Shortcut(
			config->GetShortcut("brightness_down"), this);
	shortcuts->AddShortcut("brightness_down", "Decrease Brightness",
			brightness_down);
	connect(brightness_down, &Shortcut::activated, [config = config]() {
		config->Brightness(config->Brightness() - 4);
	});
	Shortcut *brightness_up = new Shortcut(config->GetShortcut("brightness_up"),
			this);
	shortcuts->AddShortcut("brightness_up", "Increase Brightness",
			brightness_up);
	connect(brightness_up, &Shortcut::activated, [config = config]() {
		config->Brightness(config->Brightness() + 4);
	});

	Shortcut *volume_down = new Shortcut(config->GetShortcut("volume_down"),
			this);
	shortcuts->AddShortcut("volume_down", "Decrease Volume", volume_down);
	connect(volume_down, &Shortcut::activated, [config = config]() {
		config->Volume(config->Volume() - 2);
	});
	Shortcut *volume_up = new Shortcut(config->GetShortcut("volume_up"), this);
	shortcuts->AddShortcut("volume_up", "Increase Volume", volume_up);
	connect(volume_up, &Shortcut::activated, [config = config]() {
		config->Volume(config->Volume() + 2);
	});
}

QLayout* MainWindow::body() {
	QVBoxLayout *layout = new QVBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	pages->setContentsMargins(0, 0, 0, 0);

	bar->setContentsMargins(8, 0, 8, 0);
	bar->addWidget(controlsBar());

	QWidget *msg_ref = new QWidget(this);
	msg_ref->setObjectName("MsgRef");

	layout->addLayout(pages);
	layout->addWidget(msg_ref);
	layout->addLayout(bar);

	return layout;
}

void MainWindow::addPages() {
	addPage("Android Auto", openauto, "android_auto");
	addPage("OBD Viewer", obdview, "car");
	addPage("Settings", settings, "car");

	// toggle initial page
	for (QAbstractButton *button : railGroup->buttons()) {
		if (!button->isHidden()) {
			button->setChecked(true);
			break;
		}
	}

	Shortcut *shortcut = new Shortcut(config->GetShortcut("cycle_pages"), this);
	shortcuts->AddShortcut("cycle_pages", "Cycle Pages", shortcut);
	connect(shortcut, &Shortcut::activated, [this]() {
		int idx = railGroup->checkedId();
		QList<QAbstractButton*> buttons = railGroup->buttons();
		do {
			idx = (idx + 1) % buttons.size();
		} while (buttons[idx]->isHidden());
		buttons[idx]->setChecked(true);
	});
}

void MainWindow::addPage(QString name, QWidget *page, QString icon) {
	page->setObjectName(name);

	QPushButton *button = new QPushButton();
	button->setProperty("page", QVariant::fromValue(page));
	button->setCheckable(true);
	button->setFlat(true);
	button->setIconSize(Theme::icon_32);
	button->setIcon(theme->MakeButtonIcon(icon, button));

	Shortcut *shortcut = new Shortcut(config->GetShortcut(name), this);
	shortcuts->AddShortcut(name, name, shortcut);
	connect(shortcut, &Shortcut::activated, [this, button]() {
		if (!button->isHidden())
			button->setChecked(true);
	});

	int idx = pages->addWidget(page);
	railGroup->addButton(button, idx);
	rail->addWidget(button);

	button->setVisible(config->Page(page));
}

QWidget* MainWindow::controlsBar() {
	QWidget *widget = new QWidget(this);
	widget->setObjectName("ControlsBar");
	widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	QHBoxLayout *layout = new QHBoxLayout(widget);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	QPushButton *exit_button = new QPushButton(widget);
	exit_button->setFlat(true);
	exit_button->setIconSize(Theme::icon_26);

	exit_button->setIcon(theme->MakeButtonIcon("exit", exit_button));
	connect(exit_button, &QPushButton::clicked, []() {
		qApp->exit();
	});

	layout->addLayout(quickViews());
	layout->addStretch();

	layout->addWidget(exit_button);
	return widget;
}

QLayout* MainWindow::quickViews() {
	QStackedLayout *layout = new QStackedLayout();
	layout->setContentsMargins(0, 0, 0, 0);

	for (auto quickView : config->GetQuickViews().values())
		layout->addWidget(quickView);
	layout->setCurrentWidget(config->GetQuickView(config->QuickView()));
	connect(config, &Config::quickViewChanged,
			[this, layout](QString quickView) {
				layout->setCurrentWidget(config->GetQuickView(quickView));
			});

	return layout;
}

QWidget* MainWindow::controlsWidget() {
	QWidget *widget = new QWidget(this);
	QHBoxLayout *layout = new QHBoxLayout(widget);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	QPushButton *volume = new QPushButton(widget);
	volume->setFlat(true);
	volume->setIconSize(Theme::icon_26);
	volume->setIcon(theme->MakeButtonIcon("volume_up", volume));

	QLabel *volume_value = new QLabel(QString::number(config->Volume()),
			widget);
	volume_value->setFont(Theme::font_10);
	connect(config, &Config::volumeChanged, [volume_value](int volume) {
		volume_value->setText(QString::number(volume));
	});

	QPushButton *brightness = new QPushButton(widget);
	brightness->setFlat(true);
	brightness->setIconSize(Theme::icon_26);
	brightness->setIcon(theme->MakeButtonIcon("brightness_high", brightness));

	QLabel *brightness_value = new QLabel(
			QString::number((double) std::ceil(config->Brightness() / 2.55)),
			widget); // @suppress("Ambiguous problem")
	brightness_value->setFont(Theme::font_10);
	connect(config, &Config::brightnessChanged,
			[brightness_value](int brightness) {
				brightness_value->setText(
						QString::number(std::ceil((double) brightness / 2.55))); // @suppress("Ambiguous problem") // @suppress("Invalid arguments")
			});

	QPushButton *darkMode = new QPushButton(widget);
	darkMode->setFlat(true);
	darkMode->setIconSize(Theme::icon_26);
	darkMode->setIcon(theme->MakeButtonIcon("darkMode", darkMode));
	connect(darkMode, &QPushButton::clicked, [this]() {
		bool mode = !theme->Mode;
		config->DarkMode = mode;
		theme->Mode = mode;
		theme->Update();
	});

	layout->addWidget(volume, 1);
	layout->addWidget(volume_value, 7);
	layout->addStretch();
	layout->addWidget(brightness, 1);
	layout->addWidget(brightness_value, 7);
	layout->addStretch();
	layout->addWidget(darkMode, 1);

	return widget;
}

QWidget* MainWindow::powerControl() {
	QWidget *widget = new QWidget(this);
	QHBoxLayout *layout = new QHBoxLayout(widget);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	QPushButton *restart = new QPushButton(widget);
	restart->setFlat(true);
	restart->setIconSize(Theme::icon_42);
	restart->setIcon(theme->MakeButtonIcon("refresh", restart));
	connect(restart, &QPushButton::clicked, [config = config]() {
		config->save();
		sync();
		system("sudo shutdown -r now");
	});
	layout->addWidget(restart);

	QPushButton *power_off = new QPushButton(widget);
	power_off->setFlat(true);
	power_off->setIconSize(Theme::icon_42);
	power_off->setIcon(theme->MakeButtonIcon("power_settings_new", power_off));
	connect(power_off, &QPushButton::clicked, [config = config]() {
		config->save();
		sync();
		system("sudo shutdown -h now");
	});
	layout->addWidget(power_off);

	return widget;
}
