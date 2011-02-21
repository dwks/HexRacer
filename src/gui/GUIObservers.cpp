#include "GUIObservers.h"

#include "event/EventSystem.h"

namespace Project {
namespace GUI {

void GUIObservers::PauseGameObserver::observe(Event::PauseGame *event) {
    if(event->getPaused()) {
        gui->selectScreen("paused");
    }
    else {
        gui->selectScreen("running");
    }
}

void GUIObservers::SwitchToScreenObserver::observe(
    Event::SwitchToScreen *event) {
    
    gui->selectScreen(event->getScreen());
}

GUIObservers::GUIObservers(GUISystem *gui) : gui(gui) {
    ADD_OBSERVER(new PauseGameObserver(gui));
    ADD_OBSERVER(new SwitchToScreenObserver(gui));
}

}  // namespace GUI
}  // namespace Project
