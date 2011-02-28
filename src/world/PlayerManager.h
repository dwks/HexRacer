#ifndef PROJECT_WORLD__PLAYER_MANAGER_H
#define PROJECT_WORLD__PLAYER_MANAGER_H

#include "object/Player.h"
#include "object/WorldManager.h"

#include "event/TypedObserver.h"
#include "event/Enabler.h"
#include "event/PhysicsTick.h"
#include "event/WarpOntoTrack.h"
#include "event/ChangeOfIntention.h"

#include "map/RaceManager.h"

namespace Project {
namespace World {

class PlayerManager : public Event::Enabler {
protected:
    void physicsTickHandler(Event::PhysicsTick *event);
    void warpOntoTrackHandler(Event::WarpOntoTrack *event);
    void changeOfIntentionHandler(Event::ChangeOfIntention *event);
private:
    int id;
    Object::WorldManager *worldManager;
    Map::RaceManager *raceManager;
public:
    PlayerManager(int id, Object::WorldManager *worldManager);
    
    void setRaceManager(Map::RaceManager *raceManager)
        { this->raceManager = raceManager; }
public:
    Object::Player *getPlayer();
    Object::Player *getPlayer(int id);
private:
    void applyIntentions(Object::Player *player);
    void warpPlayerOntoTrack(Object::Player *player);
};

}  // namespace World
}  // namespace Project

#endif
