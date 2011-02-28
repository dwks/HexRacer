#ifndef PROJECT_WORLD__BASIC_WORLD_H
#define PROJECT_WORLD__BASIC_WORLD_H

#include "boost/shared_ptr.hpp"

#include "PlayerManager.h"

#include "map/RaceManager.h"
#include "map/PathManager.h"
#include "map/PathingUpdater.h"

#include "object/WorldManager.h"
#include "physics/PhysicsWorld.h"
#include "physics/Suspension.h"

namespace Project {
namespace World {

class BasicWorld {
private:
    boost::shared_ptr<Map::RaceManager> raceManager;
    boost::shared_ptr<PlayerManager> playerManager;
    
    boost::shared_ptr<Object::WorldManager> worldManager;
    boost::shared_ptr<Physics::PhysicsWorld> physicsWorld;
    boost::shared_ptr<Physics::Suspension> suspension;
    
    boost::shared_ptr<Map::PathManager> pathManager;
    boost::shared_ptr<Map::PathingUpdater> pathingUpdater;
public:
    void constructBeforeConnect();
    void constructDuringConnect(Object::World *world,
        Object::PlayerList *playerList, int id);
    void constructSkippingConnect();
    void constructAfterConnect(Map::HRMap *map);
    
    void doPhysics();
    void doAI();
    
    Physics::PhysicsWorld *getPhysicsWorld() { return physicsWorld.get(); }
    Object::WorldManager *getWorldManager() { return worldManager.get(); }
    Map::RaceManager *getRaceManager() { return raceManager.get(); }
    PlayerManager *getPlayerManager() { return playerManager.get(); }
    
    Map::PathManager *getPathManager() { return pathManager.get(); }
};

}  // namespace World
}  // namespace Project

#endif
