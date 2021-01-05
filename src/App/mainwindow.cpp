#include "../../include/App/mainwindow.hpp"
//#include "ui_mainwindow.h"

MainWindow::MainWindow()
{
    //this->config = Config::get_instance();
    //this->setWindowOpacity(this->config->get_brightness() / 255.0);

    //this->theme = Theme::get_instance();
    //this->theme->set_mode(this->config->get_dark_mode());
    //this->theme->set_color(this->config->get_color());

    this->openAutoView = new OpenAutoView(this);

    QWidget *widget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(widget);

    layout->addWidget(this->tabs_widget());
 //   layout->addWidget(this->controls_widget());

    setCentralWidget(widget); 
}


MainWindow::~MainWindow()
{
}

QTabWidget *MainWindow::tabs_widget()
{
    QTabWidget *widget = new QTabWidget(this);
    widget->setTabPosition(QTabWidget::TabPosition::West);
    //widget->tabBar()->setIconSize(Theme::icon_48);
    //widget->tabBar()->setIconSize(20);

    widget->addTab(this->openAutoView, QString());
    //this->theme->add_tab_icon("directions_car", 0, Qt::Orientation::Vertical);
    /*
    connect(this->config, &Config::brightness_changed, [this, widget](int position) {
        this->setWindowOpacity(position / 255.0);
        if (widget->currentIndex() == 0) emit set_open_auto_state(position);
    });
    /*
    connect(this->theme, &Theme::icons_updated,
        [widget](QList<tab_icon_t> &tab_icons, QList<button_icon_t> &button_icons) {
            for(auto &icon : tab_icons) widget->tabBar()->setTabIcon(icon.first, icon.second);
            for(auto &icon : button_icons) icon.first->setIcon(icon.second);
        });
        
    connect(widget, &QTabWidget::currentChanged, [this](int index) {
        emit set_open_auto_state((index == 0) ? (windowOpacity() * 255) : 0);
        emit set_data_state(index == 2);
    });
    */
    return widget;
}

/*QWidget * MainWindow::control_widget()
{
    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);
 //   layout->setContentsMargins(0,0,0,0);
 //   layout->addWidget(this->volume_widget(), 3);

    return widget;
}
* */
/*
QWidget *MainWindow::volume_widget()
{
    QWidget *widget = new QWidget(this);

}
*/

void MainWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    this->openAutoView->startWorker();
    this->openAutoView->setFocus();
}


