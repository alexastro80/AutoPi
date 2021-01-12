#include <QDir>
#include <QElapsedTimer>
#include <QTimer>

#include <QDebug>

#include "App/shortcuts.hpp"

static const QRegExp GPIOX_REGEX("gpio\\d+");
static const QString GPIO_DIR("/sys/class/gpio");
static const QString GPIOX_DIR(GPIO_DIR + "/%1");
static const QString GPIOX_VALUE_PATH(GPIOX_DIR + "/value");
static const QString GPIOX_ACTIVE_LOW_PATH(GPIOX_DIR + "/active_low");

GpioWatcher::GpioWatcher(QObject *parent) : QObject(parent)
{
    watcher = new QFileSystemWatcher(this);
    for (auto gpio : QDir(GPIO_DIR).entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        if (GPIOX_REGEX.exactMatch(gpio) && QFile(GPIOX_ACTIVE_LOW_PATH.arg(gpio)).exists())
            watcher->addPath(GPIOX_VALUE_PATH.arg(gpio));
    }

    connect(watcher, &QFileSystemWatcher::fileChanged,
            [this](QString path) { emit gpioTriggered(QFileInfo(path).dir().dirName()); });

    disable();
}

ShortcutInput::ShortcutInput(QString shortcut, QWidget *parent) : QPushButton(shortcut, parent)
{
    gpioWatcher = new GpioWatcher(this);
    connect(gpioWatcher, &GpioWatcher::gpioTriggered, [this](QString gpio) {
        setText(gpio);
        emit shortcutUpdated(gpio);
    });

    QElapsedTimer *timer = new QElapsedTimer();
    connect(this, &QPushButton::pressed, [timer]() { timer->start(); });
    connect(this, &QPushButton::released, [this, timer]() {
        if (timer->hasExpired(500)) {
            setText(QString());
            emit shortcutUpdated(text());
        }
    });
}

void ShortcutInput::keyPressEvent(QKeyEvent *event)
{
    Qt::Key k = static_cast<Qt::Key>(event->key());
    if (k == Qt::Key_unknown || k == Qt::Key_Control || k == Qt::Key_Shift || k == Qt::Key_Alt || k == Qt::Key_Meta)
        return;

    QKeySequence shortcut(event->modifiers() + k);
    setText(shortcut.toString());
    emit shortcutUpdated(text());
}

Shortcut::Shortcut(QString shortcutStr, QWidget *parent) : QObject(parent), gpioValueAttribute(parent)
{
    shortcut = shortcutStr;
    key = new QShortcut(parent);
    gpio = new QFileSystemWatcher(parent);

    connect(gpio, &QFileSystemWatcher::fileChanged, [this](QString) {
        if (gpioValueAttribute.isOpen()) {
            gpioValueAttribute.seek(0);
            if (gpioActiveLow == gpioValueAttribute.read(1).at(0)) {
                gpio->blockSignals(true);
                emit activated();
                QTimer::singleShot(300, [gpio = gpio]() { gpio->blockSignals(false); });
            }
            else {
                qDebug() << "[Dash][Shortcut]" << shortcut << ": active low != value"; // temp
            }
        }
        else {
            qDebug() << "[Dash][Shortcut]" << shortcut << ":" << gpioValueAttribute.fileName() << "is not open"; // temp
        }
    });
    connect(key, &QShortcut::activated, [this]() { emit activated(); });
}

Shortcut::~Shortcut()
{
    if (gpioValueAttribute.isOpen()) gpioValueAttribute.close();
}

void Shortcut::SetShortcut(QString shortcut)
{
    key->setKey(QKeySequence());

    QStringList gpios = gpio->files();
    if (!gpios.isEmpty()) gpio->removePaths(gpios);
    if (gpioValueAttribute.isOpen()) gpioValueAttribute.close();

    shortcut = shortcut;
    if (shortcut.startsWith("gpio")) {
        qDebug() << "[Dash][Shortcut]" << shortcut << ": setting shortcut as gpio"; // temp
        gpioValueAttribute.setFileName(GPIOX_VALUE_PATH.arg(shortcut));
        if (gpioValueAttribute.open(QIODevice::ReadOnly)) {
            QFile active_low_attribute(GPIOX_ACTIVE_LOW_PATH.arg(shortcut));
            if (active_low_attribute.open(QIODevice::ReadOnly)) {
                gpioActiveLow = active_low_attribute.read(1)[0];
                active_low_attribute.close();
                gpio->addPath(gpioValueAttribute.fileName());
            }
            else {
                qDebug() << "[Dash][Shortcut]" << shortcut << ": failed to open" << active_low_attribute; // temp
            }
        }
        else {
            qDebug() << "[Dash][Shortcut]" << shortcut << ": failed to open" << gpioValueAttribute.fileName(); // temp
        }
    }
    else if (!shortcut.isNull()) {
        qDebug() << "[Dash][Shortcut]" << shortcut << ": setting shortcut as key"; // temp
        key->setKey(QKeySequence::fromString(shortcut));
    }
}

void Shortcuts::AddShortcut(QString id, QString description, Shortcut *shortcut)
{
    shortcuts[id] = {description, shortcut};
    emit shortcutAdded(id, description, shortcut);
}

void Shortcuts::InitShortcuts()
{
    for (auto shortcut : shortcuts)
        shortcut.second->InitShortcut();
}

Shortcuts *Shortcuts::getInstance()
{
    static Shortcuts shortcuts;
    return &shortcuts;
}
