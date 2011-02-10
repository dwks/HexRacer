#include "GUIObservers.h"

#include "event/ObserverList.h"

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

GUIObservers::GUIObservers(GUISystem *gui) : gui(gui) {
    ADD_OBSERVER(new PauseGameObserver(gui));
}

}  // namespace GUI
}  // namespace Project
