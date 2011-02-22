#ifndef PROJECT_SDL__PLAYER_MANAGER_H
#define PROJECT_SDL__PLAYER_MANAGER_H

#include "object/Player.h"
#include "object/WorldManager.h"

#include "physics/Suspension.h"

#include "event/TypedObserver.h"
#include "event/Enabler.h"
#include "event/PlayerAction.h"
#include "event/PaintEvent.h"

#include "map/RaceManager.h"

#include "opengl/Color.h"

namespace Project {
namespace SDL {

class PlayerManager : public Event::Enabler {
private:
    int id;
    Object::WorldManager *worldManager;
    Map::RaceManager *raceManager;
private:
    class PlayerActionHandler
        : public Event::TypedObserver<Event::PlayerAction> {
    private:
        PlayerManager *manager;
    public:
        PlayerActionHandler(PlayerManager *manager) : manager(manager) {}
        
        virtual void observe(Event::PlayerAction *action);
    };
public:
    PlayerManager(int id, Object::WorldManager *worldManager);
    
    void setRaceManager(Map::RaceManager *raceManager)
        { this->raceManager = raceManager; }
public:
    Object::Player *getPlayer();
    Object::Player *getPlayer(int id);
};

}  // namespace SDL
}  // namespace Project

#endif
