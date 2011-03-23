#include <cstdlib>
#include <ctime>

#include "ServerMain.h"
#include "log/LogOpener.h"
#include "settings/ProgramSettings.h"
#include "settings/SettingsManager.h"

#include "config.h"

int main(int argc, char *argv[]) {
    std::srand(std::time(0));
    
    Project::Log::LogOpener::openLogs();
    Project::Settings::ProgramSettings programSettings(false);
    Project::Settings::SettingsManager settings(CONFIG_FILE);
    
    for(int arg = 1; arg < argc; arg ++) {
        settings.parse(argv[arg]);
    }
    
    Project::Server::ServerMain *server = new Project::Server::ServerMain();
    server->run();
    delete server;
    
    Project::Log::LogOpener::closeLogs();
    return 0;
}
