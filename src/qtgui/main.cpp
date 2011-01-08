#include <QApplication>
#include "MainWindow.h"
#include "event/ObserverList.h"
#include "log/LogOpener.h"

int main(int argc, char *argv[]) {
    Project::Log::LogOpener::openLogs();
    
    QApplication app(argc, argv);
    
    Project::QtGUI::MainWindow main;
    main.show();
    
    int returnValue = app.exec();
    Project::Log::LogOpener::closeLogs();
    Project::Event::ObserverList::getInstance().destroyObject();
    return returnValue;
}
