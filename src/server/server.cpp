#include "ServerMain.h"
#include "event/ObserverList.h"
#include "log/LogOpener.h"
#include "settings/ProgramSettings.h"
#include "settings/SettingsManager.h"

#include "config.h"

int main(int argc, char *argv[]) {
    Project::Log::LogOpener::openLogs();
    Project::Settings::ProgramSettings programSettings(false);
    Project::Settings::SettingsManager settings(CONFIG_FILE);
    
    Project::Server::ServerMain server;
    server.run();
    
    Project::Event::ObserverList::getInstance().destroyObject();
    
    Project::Log::LogOpener::closeLogs();
    return 0;
}
