#include "SDLMainLoop.h"
#include "log/LogOpener.h"
#include "settings/ProgramSettings.h"
#include "settings/SettingsManager.h"

#include "config.h"

void runGame() {

	Project::Settings::ProgramSettings programSettings(true);
    Project::Settings::SettingsManager settings(CONFIG_FILE);
    
    Project::SDL::SDLMainLoop *sdlmain = new Project::SDL::SDLMainLoop();
    sdlmain->run();
    delete sdlmain;

}

int main(int argc, char *argv[]) {
    Project::Log::LogOpener::openLogs();
	runGame();
    Project::Log::LogOpener::closeLogs();
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev,
    LPSTR lpszCmndline, int nCmdShow) {
	runGame();
	return 0;
}