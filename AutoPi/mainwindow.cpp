#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QProcess>
#include "../../openauto/include/f1x/openauto/autoapp/App.hpp"
#include "openauto.hpp"
SubWindow* openAuto = nullptr;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    openAuto = new SubWindow(ui->openAuto);
    //QString openAutoPath = "../openauto/openauto.sh";
    QString openAutoPath = "C:/Program Files/PuTTY/putty.exe";
    QStringList args;
//    args << ""

    openAuto->start(openAutoPath, args);


}
bool MainWindow::event(QEvent *e)
{
    if (e->type() == QEvent::WindowActivate) {
        //Get mouse location.

        openAuto->Activate();
    }
    return QWidget::event(e);
    return true;
}

MainWindow::~MainWindow()
{
    delete openAuto;
    delete ui;
}
SubWindow::SubWindow(QObject* parent) : QProcess(parent)
{}
void SubWindow::Activate()
{
    setProcessState(ProcessState::Running);
}

