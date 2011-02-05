#ifndef PROJECT_SDL__MENU_LOOP_H
#define PROJECT_SDL__MENU_LOOP_H

#include "boost/smart_ptr/shared_ptr.hpp"

#include "LoopBase.h"

#include "gui/GUISystem.h"
#include "GUIInputManager.h"

namespace Project {
namespace SDL {

class MenuLoop : public LoopBase {
private:
    boost::shared_ptr<GUI::GUISystem> gui;
    boost::shared_ptr<GUIInputManager> guiInputManager;
public:
    virtual void construct();
    
    virtual void handleEvent(SDL_Event *event);
    virtual void render();
    
    virtual void setProjection(const Point2D &size) {}
};

}  // namespace SDL
}  // namespace Project

#endif
