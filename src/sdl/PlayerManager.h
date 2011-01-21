#ifndef PROJECT_SDL__PLAYER_MANAGER_H
#define PROJECT_SDL__PLAYER_MANAGER_H

#include "object/Player.h"
#include "object/PlayerList.h"

#include "event/TypedObserver.h"
#include "event/PlayerMovement.h"
#include "event/UpdatePlayerList.h"

#include "opengl/Color.h"

namespace Project {
namespace SDL {

class PlayerManager {
private:
    int id;
    Object::PlayerList *playerList;
private:
    class PlayerMovementHandler
        : public Event::TypedObserver<Event::PlayerMovement> {
    private:
        PlayerManager *manager;
    public:
        PlayerMovementHandler(PlayerManager *manager) : manager(manager) {}
        
        virtual void observe(Event::PlayerMovement *movement);
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
    PlayerManager(int id);
    ~PlayerManager();
    
    void applySuspension(Render::RenderManager *renderManager);
    
    void render(Render::RenderManager *renderManager);
	OpenGL::Color getPlayerColor(int id);
protected:
    void usePlayerList(Object::PlayerList *playerList);
    Object::Player *getPlayer();
};

}  // namespace SDL
}  // namespace Project

#endif
