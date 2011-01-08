#include "SDLMain.h"
#include "log/LogOpener.h"

int main(int argc, char *argv[]) {
    Project::Log::LogOpener::openLogs();
    
    Project::SDL::SDLMain sdlmain;
    sdlmain.run();
    
    Project::Log::LogOpener::closeLogs();
    
    return 0;
}
