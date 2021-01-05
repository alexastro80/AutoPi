#include"../../include/Views/openautotab.hpp"
//#include "Views/openautotab.hpp"

OpenAutoWorker::OpenAutoWorker(std::function<void(bool)> callback, QWidget)
    : io_service(),
      work(io_service),
//      configuration(Config::get_instance()->open_auto_config),
      tcpWrapper(),

