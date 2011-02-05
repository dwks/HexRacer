#include "MenuLoop.h"

namespace Project {
namespace SDL {

void MenuLoop::construct() {
    gui = boost::shared_ptr<GUI::GUISystem>(
        new GUI::GUISystem());
    guiInputManager = boost::shared_ptr<GUIInputManager>(
        new GUIInputManager(gui));
    
    gui->construct();
    gui->selectScreen("main");
}

void MenuLoop::handleEvent(SDL_Event *event) {
    guiInputManager->handleEvent(event);
}

void MenuLoop::render() {
    gui->render();
}

}  // namespace SDL
}  // namespace Project
