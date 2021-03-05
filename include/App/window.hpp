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

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow();
    void AddWidget(QWidget *widget);

    inline QList<QAbstractButton *> Pages() { return railGroup->buttons(); }

   protected:
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

   private:
    Config *config;
    Theme *theme;
    Shortcuts *shortcuts;

    View* openauto;
    OpenAuto* openautoWorker;
    View* obdview;
    OBDWorker* obdWorker;
    QStackedWidget *stack;
    QVBoxLayout *rail;
    QButtonGroup *railGroup;
    QStackedLayout *pages;
    QHBoxLayout *bar;

    void initConfig();
    void initTheme();
    void initUI();
    void initShortcuts();
    QLayout* body();
    void addPages();
    void addPage(QString name, QWidget *page, QString icon);

    QWidget *controlsBar();
    QLayout *quickViews();
    QWidget *brightnessWidget(bool skip_buttons = false);
    QWidget *controlsWidget();
    QWidget *powerControl();
    QWidget *saveControl();
};
