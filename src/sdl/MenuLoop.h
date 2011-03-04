#ifndef PROJECT_SDL__MENU_LOOP_H
#define PROJECT_SDL__MENU_LOOP_H

#include "boost/smart_ptr/shared_ptr.hpp"

#include "LoopBase.h"

#include "gui/GUISystem.h"
#include "GUIInputManager.h"
#include "MenuBackground.h"

#include "misc/ProgressTracker.h"

namespace Project {
namespace SDL {

class MenuLoop : public LoopBase {
private:
    boost::shared_ptr<GUI::GUISystem> gui;
    boost::shared_ptr<GUIInputManager> guiInputManager;
    boost::shared_ptr<MenuBackground> menuBackground;

	static Misc::ProgressTracker* loadingProgressTracker;
public:
    virtual void construct();
    
    virtual void handleEvent(SDL_Event *event);
    virtual void miscellaneous() {}
    virtual void render();
    
    boost::shared_ptr<GUI::GUISystem> getGUI() { return gui; }
    boost::shared_ptr<GUIInputManager> getGUIInput()
        { return guiInputManager; }

	static Misc::ProgressTracker* getLoadingProgressTracker() { return loadingProgressTracker; }
    
    virtual void setProjection(const Point2D &size) {}
};

}  // namespace SDL
}  // namespace Project

#endif
