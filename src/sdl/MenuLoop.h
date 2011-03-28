#ifndef PROJECT_SDL__MENU_LOOP_H
#define PROJECT_SDL__MENU_LOOP_H

#include "boost/smart_ptr/shared_ptr.hpp"

#include "LoopBase.h"

#include "gui/GUISystem.h"
#include "GUIInputManager.h"
#include "MenuBackground.h"

#include "misc/ProgressTracker.h"
#include "map/MapSettings.h"

namespace Project {
namespace SDL {

class MenuLoop : public LoopBase {
private:
    boost::shared_ptr<GUI::GUISystem> gui;
    boost::shared_ptr<GUIInputManager> guiInputManager;
    boost::shared_ptr<MenuBackground> menuBackground;
    
    boost::shared_ptr<Map::MapSettings> mapSettings;

	static Misc::ProgressTracker* loadingProgressTracker;
public:
    virtual void construct();
    
    virtual void handleEvent(SDL_Event *event);
    virtual void miscellaneous();
    virtual void render();
    virtual void postRender();
    
    boost::shared_ptr<GUI::GUISystem> getGUI() { return gui; }
    boost::shared_ptr<GUIInputManager> getGUIInput()
        { return guiInputManager; }

	static Misc::ProgressTracker* getLoadingProgressTracker() { return loadingProgressTracker; }
    
    virtual void setProjection(const Point2D &size) {}
};

}  // namespace SDL
}  // namespace Project

#endif
