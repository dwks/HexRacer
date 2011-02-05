#ifndef PROJECT_SDL__GUIINPUT_MANAGER_H
#define PROJECT_SDL__GUIINPUT_MANAGER_H

#include "SDL_events.h"

#include "widget/GUISystem.h"

namespace Project {
namespace SDL {

class GUIInputManager {
private:
    Widget::GUISystem *gui;
public:
    GUIInputManager(Widget::GUISystem *gui) : gui(gui) {}
    
    void handleEvent(SDL_Event *event);
};

}  // namespace SDL
}  // namespace Project

#endif
