#pragma once

#include <QApplication>
#include <QFile>
#include <QFileSystemWatcher>
#include <QKeyEvent>
#include <QPushButton>
#include <QShortcut>
#include <QString>
#include <QWidget>

class GpioWatcher : public QObject {
    Q_OBJECT

   public:
    GpioWatcher(QObject *parent = nullptr);

    inline void enable() { watcher->blockSignals(false); }
    inline void disable() { watcher->blockSignals(true); }

   private:
    QFileSystemWatcher *watcher;

   signals:
    void gpioTriggered(QString gpio);
};

class ShortcutInput : public QPushButton {
    Q_OBJECT

   public:
    ShortcutInput(QString shortcut, QWidget *parent = nullptr);

   protected:
    void keyPressEvent(QKeyEvent *event);
    inline void focusInEvent(QFocusEvent *) { gpioWatcher->enable(); }
    inline void focusOutEvent(QFocusEvent *) { gpioWatcher->disable(); }

   private:
    GpioWatcher *gpioWatcher;

   signals:
    void shortcutUpdated(QString shortcut);
};

class Shortcut : public QObject {
    Q_OBJECT

   public:
    Shortcut(QString shortcut, QWidget *parent);
    ~Shortcut();

    inline QString ToString() { return shortcut; }
    void SetShortcut(QString shortcut);
    inline void InitShortcut() { SetShortcut(shortcut); }

   private:
    QString shortcut;
    QShortcut *key;
    QFileSystemWatcher *gpio;
    QFile gpioValueAttribute;
    int gpioActiveLow;

   signals:
    void activated();
};

class Shortcuts : public QObject {
    Q_OBJECT

   public:
    Shortcuts() : QObject(qApp) {}

    void AddShortcut(QString id, QString description, Shortcut *shortcut);
    void InitShortcuts();

    inline void UpdateShortcut(QString id, QString shortcut) { shortcuts[id].second->SetShortcut(shortcut); }
    inline QMap<QString, QPair<QString, Shortcut *>> GetShortcuts() { return shortcuts; }

    static Shortcuts *getInstance();

   private:
    QMap<QString, QPair<QString, Shortcut *>> shortcuts;

   signals:
    void shortcutAdded(QString id, QString description, Shortcut *shortcut);
};
