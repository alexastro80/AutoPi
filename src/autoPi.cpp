#include <QApplication>
#include <QStringList>
#include <QWindow>

#include "App/window.hpp"

int main(int argc, char *argv[])
{
     QApplication autoPi(argc, argv);
     QSplashScreen splash;

     QStringList args = autoPi.arguments();
     bool use_fixed_size = (args.size() > 2);
     
     QSize size = autoPi.primaryScreen()->size();
     if (use_fixed_size)
         size = QSize(args.at(1).toInt(), args.at(2).toInt());
     system("pwd");
     //Launch input if necessary
//     system("./lib/scripts/Input/input.sh &");
          
     //Launch OBD.py
     //system("./lib/scripts/OBD/obd.sh &");
     
     splash.setPixmap(QPixmap(":/splash.png").scaled(size, Qt::KeepAspectRatio));
     splash.show();
     autoPi.processEvents();

     MainWindow window;
     window.setWindowIcon(QIcon(":/logo.png"));
     window.setWindowFlags(Qt::FramelessWindowHint);
     if (!use_fixed_size)
         window.setWindowState(Qt::WindowFullScreen);

     // force to either screen or custom size
     window.setFixedSize(size);
     window.show();
     splash.finish(&window);
     
     
     return autoPi.exec();
}
