#include "SDLMainLoop.h"
#include "log/LogOpener.h"
#include "settings/ProgramSettings.h"
#include "settings/SettingsManager.h"

#include "config.h"

#if defined(WIN32) && defined(WIN32_NO_CONSOLE)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#else
int main(int argc, char *argv[]) {
#endif
    Project::Log::LogOpener::openLogs();
    Project::Settings::ProgramSettings programSettings(true);
    Project::Settings::SettingsManager settings(CONFIG_FILE);
    
    Project::SDL::SDLMainLoop *sdlmain = new Project::SDL::SDLMainLoop();
    sdlmain->run();
    delete sdlmain;
    
    Project::Log::LogOpener::closeLogs();
    return 0;
}

#ifdef WIN32
#endif