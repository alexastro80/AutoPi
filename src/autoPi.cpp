#include <QApplication>
#include <QStringList>
#include <QWindow>

#include "App/window.hpp"

int main(int argc, char *argv[])
{
     QApplication autoPi(argc, argv);
     QSplashScreen splash;

     QString thisPath = QCoreApplication::applicationDirPath();
     thisPath = thisPath.left(thisPath.length()-3);
     chdir(thisPath.toStdString().c_str());
     std::cout << QDir::currentPath().toStdString() << "\n";
     QStringList args = autoPi.arguments();
     bool use_fixed_size = (args.size() > 2);
     
     QSize size = autoPi.primaryScreen()->size();
     if (use_fixed_size)
         size = QSize(args.at(1).toInt(), args.at(2).toInt());

     splash.setPixmap(QPixmap(":/splash.png").scaled(size, Qt::KeepAspectRatio));
     splash.show();
     autoPi.processEvents();

     MainWindow window;
     window.setWindowIcon(QIcon(":/logo.png")); // @suppress("Ambiguous problem")
     window.setWindowFlags(Qt::FramelessWindowHint); // @suppress("Ambiguous problem")
     if (!use_fixed_size)
 //        window.setWindowState(Qt::WindowFullScreen);

     // force to either screen or custom size
     window.setFixedSize(size); // @suppress("Ambiguous problem")
     window.show(); // @suppress("Ambiguous problem")
     splash.finish(&window);
     
     
     return autoPi.exec();
}
