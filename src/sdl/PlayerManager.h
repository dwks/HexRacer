#ifndef PROJECT_SDL__PLAYER_MANAGER_H
#define PROJECT_SDL__PLAYER_MANAGER_H

#include "object/Player.h"
#include "object/PlayerList.h"

#include "event/TypedObserver.h"
#include "event/PlayerMovement.h"

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
        Object::Player *player;
    public:
        PlayerMovementHandler(Object::Player *player) : player(player) {}
        
        virtual void observe(Event::PlayerMovement *movement);
    };
public:
    PlayerManager(int id);
    ~PlayerManager();
    
    void render();
};

}  // namespace SDL
}  // namespace Project

#endif
