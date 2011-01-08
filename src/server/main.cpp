#include "ServerMain.h"
#include "log/LogOpener.h"

int main(int argc, char *argv[]) {
    Project::Log::LogOpener::openLogs();
    
    Project::Server::ServerMain server;
    server.run();
    
    Project::Log::LogOpener::closeLogs();
    return 0;
}
