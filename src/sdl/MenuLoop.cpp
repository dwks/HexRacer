#include "MenuLoop.h"

namespace Project {
namespace SDL {

Misc::ProgressTracker* MenuLoop::loadingProgressTracker = NULL;

void MenuLoop::construct() {
    gui = boost::shared_ptr<GUI::GUISystem>(
        new GUI::GUISystem());
    guiInputManager = boost::shared_ptr<GUIInputManager>(
        new GUIInputManager(gui));
    
    gui->construct();
    gui->selectScreen("main");

	loadingProgressTracker = dynamic_cast<Misc::ProgressTracker*>
		(gui->getWidget("main/progressBar"));
    
    menuBackground = boost::shared_ptr<MenuBackground>(
        new MenuBackground());
}

void MenuLoop::handleEvent(SDL_Event *event) {
    guiInputManager->handleEvent(event);
}

void MenuLoop::render() {
    menuBackground->render();
    
    gui->render();
}

}  // namespace SDL
}  // namespace Project
