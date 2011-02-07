#ifndef PROJECT_SDL__GUIINPUT_MANAGER_H
#define PROJECT_SDL__GUIINPUT_MANAGER_H

#include "boost/smart_ptr/shared_ptr.hpp"

#include "SDL_events.h"

#include "gui/GUISystem.h"

namespace Project {
namespace SDL {

class GUIInputManager {
private:
    boost::shared_ptr<GUI::GUISystem> gui;
public:
    GUIInputManager(boost::shared_ptr<GUI::GUISystem> gui) : gui(gui) {}
    
    void handleEvent(SDL_Event *event);
};

}  // namespace SDL
}  // namespace Project

#endif
