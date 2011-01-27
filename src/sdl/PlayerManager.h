#ifndef PROJECT_SDL__PLAYER_MANAGER_H
#define PROJECT_SDL__PLAYER_MANAGER_H

#include "object/Player.h"
#include "object/PlayerList.h"
#include "object/WorldManager.h"

#include "physics/Suspension.h"

#include "event/TypedObserver.h"
#include "event/PlayerAction.h"
#include "event/UpdatePlayerList.h"
#include "event/PaintEvent.h"

#include "paint/PaintManager.h"

#include "opengl/Color.h"

namespace Project {
namespace SDL {

class PlayerManager {
private:
    int id;
    Object::WorldManager *worldManager;
    Physics::Suspension suspension;
private:
    class PlayerActionHandler
        : public Event::TypedObserver<Event::PlayerAction> {
    private:
        PlayerManager *manager;
    public:
        PlayerActionHandler(PlayerManager *manager) : manager(manager) {}
        
        virtual void observe(Event::PlayerAction *action);
    };
    
    class UpdatePlayerListHandler
        : public Event::TypedObserver<Event::UpdatePlayerList> {
    private:
        PlayerManager *manager;
    public:
        UpdatePlayerListHandler(PlayerManager *manager) : manager(manager) {}
        
        virtual void observe(Event::UpdatePlayerList *update);
    };
public:
    PlayerManager(int id, Object::WorldManager *worldManager);
    ~PlayerManager();
    
    void applySuspension(Render::RenderManager *renderManager);
    
protected:
    void usePlayerList(Object::PlayerList *playerList);
public:
    Object::Player *getPlayer();
    Object::Player *getPlayer(int id);
};

}  // namespace SDL
}  // namespace Project

#endif
