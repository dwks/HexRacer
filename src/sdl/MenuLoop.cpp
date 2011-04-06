#include "MenuLoop.h"

#include "widget/RepaintEvent.h"
#include "misc/ProgressLogger.h"

namespace Project {
namespace SDL {

Misc::ProgressTracker* MenuLoop::loadingProgressTracker = NULL;

void MenuLoop::construct() {
    mapSettings = boost::shared_ptr<Map::MapSettings>(new Map::MapSettings());
    
    gui = boost::shared_ptr<GUI::GUISystem>(
        new GUI::GUISystem());
    guiInputManager = boost::shared_ptr<GUIInputManager>(
        new GUIInputManager(gui));
    
    gui->construct();
    
	loadingProgressTracker = new Misc::ProgressLogger();
    
	/*
	loadingProgressTracker = dynamic_cast<Misc::ProgressTracker*>
		(gui->getWidget("main/progressBar"));
		*/
    
    menuBackground = boost::shared_ptr<MenuBackground>(
        new MenuBackground());
    
#ifdef HAVE_OPENAL
    if(GET_SETTING("sound.enable", 0)==1){
        soundSystem = boost::shared_ptr<Sound::SoundSystem>(new Sound::SoundSystem());
        soundSystem->initialize();
    }
#endif
}

void MenuLoop::handleEvent(SDL_Event *event) {
    guiInputManager->handleEvent(event);
	menuBackground->handleEvent(event);
}

void MenuLoop::render() {
    menuBackground->render();
    
    gui->render();
}

void MenuLoop::postRender() {
    // fire a repaint event to the current screen
    Widget::RepaintEvent repaintEvent(gui->getCurrentScreen());
    gui->handleEvent(&repaintEvent);
#ifdef HAVE_OPENAL
        if (GET_SETTING("sound.enable", true)) {
                soundSystem->doAction();
        }
#endif

}

void MenuLoop::miscellaneous() {
	guiInputManager->generateMenuEvents();
    
    if(!gameWorld.expired()) {
        gameWorld.lock()->checkNetwork();
    }
}

}  // namespace SDL
}  // namespace Project
