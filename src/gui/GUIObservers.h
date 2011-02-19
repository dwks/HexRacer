#ifndef PROJECT_GUI__GUIOBSERVERS_H
#define PROJECT_GUI__GUIOBSERVERS_H

#include "GUISystem.h"

#include "event/TypedObserver.h"
#include "event/PauseGame.h"
#include "event/SwitchToScreen.h"

namespace Project {
namespace GUI {

class GUIObservers {
private:
    GUISystem *gui;
private:
    class PauseGameObserver : public Event::TypedObserver<Event::PauseGame> {
    private:
        GUISystem *gui;
    public:
        PauseGameObserver(GUISystem *gui) : gui(gui) {}
        
        virtual void observe(Event::PauseGame *event);
    };
    
    class SwitchToScreenObserver
        : public Event::TypedObserver<Event::SwitchToScreen> {
    private:
        GUISystem *gui;
    public:
        SwitchToScreenObserver(GUISystem *gui) : gui(gui) {}
        
        virtual void observe(Event::SwitchToScreen *event);
    };
public:
    GUIObservers(GUISystem *gui);
};

}  // namespace GUI
}  // namespace Project

#endif
