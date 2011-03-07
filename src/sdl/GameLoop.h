#ifndef PROJECT_SDL__GAME_LOOP_H
#define PROJECT_SDL__GAME_LOOP_H

#include <string>

#include "boost/smart_ptr/shared_ptr.hpp"

#include "LoopBase.h"

#include "GameViewport.h"
#include "GameRenderer.h"
#include "GameWorld.h"

#include "gui/GUISystem.h"
#include "GUIInputManager.h"

#include "paint/PaintSubsystem.h"
#include "InputManager.h"

namespace Project {
namespace SDL {

class GameLoop : public LoopBase {
private:
    boost::shared_ptr<GameViewport> viewport;
    boost::shared_ptr<GameRenderer> gameRenderer;
    boost::shared_ptr<GameWorld> gameWorld;
    
    boost::shared_ptr<Paint::PaintSubsystem> paintSubsystem;
    boost::shared_ptr<InputManager> inputManager;
    
    boost::shared_ptr<GUI::GUISystem> gui;
    boost::shared_ptr<GUIInputManager> guiInputManager;
public:
    virtual bool tryConnect(const std::string &host, unsigned short port);
    
    virtual void construct();
    
    void setGuiPointers(boost::shared_ptr<GUI::GUISystem> gui,
        boost::shared_ptr<GUIInputManager> guiInputManager);
    
    virtual void handleEvent(SDL_Event *event);
    virtual void miscellaneous();
    virtual void render();
    virtual void postRender();
    
    virtual void setProjection(const Point2D &size);
};

}  // namespace SDL
}  // namespace Project

#endif
