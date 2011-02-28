#include "SDLMainLoop.h"
#include "log/LogOpener.h"
#include "settings/ProgramSettings.h"
#include "settings/SettingsManager.h"

#include "config.h"

int main(int argc, char *argv[]) {
    Project::Log::LogOpener::openLogs();
    Project::Settings::ProgramSettings programSettings(true);
    Project::Settings::SettingsManager settings(CONFIG_FILE);
    
    Project::SDL::SDLMainLoop *sdlmain = new Project::SDL::SDLMainLoop();
    sdlmain->run();
    delete sdlmain;
    
    Project::Log::LogOpener::closeLogs();
    return 0;
}
