#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include "../Views/openAutoView.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
protected:
    void showEvent(QShowEvent *event);


private:
 //   static void update_system_volume(int position);
    QTabWidget *tabs_widget();
 //   QWidget *controls_widget();
 //   QWidget *volume_widget();

 //   Config *config;
 //   Theme *theme;
    OpenAutoView *openAutoView;
signals:
    void set_data_state(bool enabled);
    void set_open_auto_state(unsigned int alpha);
};

#endif // MAINWINDOW_HPP

