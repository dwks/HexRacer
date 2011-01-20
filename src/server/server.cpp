#include "ServerMain.h"
#include "event/ObserverList.h"
#include "log/LogOpener.h"
#include "settings/SettingsManager.h"

int main(int argc, char *argv[]) {
    Project::Log::LogOpener::openLogs();
    Project::Settings::SettingsManager settings("config.txt");
    
    Project::Server::ServerMain server;
    server.run();
    
    Project::Event::ObserverList::getInstance().destroyObject();
    
    Project::Log::LogOpener::closeLogs();
    return 0;
}
