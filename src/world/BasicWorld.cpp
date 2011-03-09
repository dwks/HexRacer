#include "BasicWorld.h"

#include "log/Logger.h"
#include "timing/AccelControl.h"

#include "misc/Sleeper.h"

namespace Project {
namespace World {

void BasicWorld::constructBeforeConnect() {
    physicsWorld = boost::shared_ptr<Physics::PhysicsWorld>(
        new Physics::PhysicsWorld());
    suspension = boost::shared_ptr<Physics::Suspension>(
        new Physics::Suspension());
}

void BasicWorld::constructDuringConnect(Object::World *world,
    Object::PlayerList *playerList, int id) {
    
    worldManager = boost::shared_ptr<Object::WorldManager>(
        new Object::WorldManager(world, playerList));
    playerManager = boost::shared_ptr<PlayerManager>(
        new PlayerManager(id, worldManager.get()));
}

void BasicWorld::constructSkippingConnect() {
    worldManager = boost::shared_ptr<Object::WorldManager>(
        new Object::WorldManager());
    playerManager = boost::shared_ptr<PlayerManager>(
        new PlayerManager(0, worldManager.get()));
}

void BasicWorld::constructAfterConnect(Map::HRMap *map) {
    raceManager = boost::shared_ptr<Map::RaceManager>(
        new Map::RaceManager(map));
    pathManager = boost::shared_ptr<Map::PathManager>(
        new Map::PathManager(map->getMapObjects().getPathNodes()));
    pathingUpdater = boost::shared_ptr<Map::PathingUpdater>(
        new Map::PathingUpdater(worldManager, raceManager, playerManager));
    
    worldManager->setPathManager(pathManager.get());
    
    playerManager->setRaceManager(raceManager.get());
    
    suspension->setData(worldManager.get());
}

void BasicWorld::doPhysics() {
    suspension->checkForWheelsOnGround();
    
    if(!Timing::AccelControl::getInstance()->getPaused()) {
        static unsigned long lastPhysicsTime
            = Misc::Sleeper::getTimeMilliseconds();
        lastPhysicsTime += Timing::AccelControl::getInstance()->getPauseSkip();
        unsigned long thisTime = Misc::Sleeper::getTimeMilliseconds();
        
        /*LOG(PHYSICS, "step physics by "
            << (thisTime - lastPhysicsTime) * 1000);*/
        
        physicsWorld->stepWorld((thisTime - lastPhysicsTime));
        lastPhysicsTime = thisTime;
    }
}

void BasicWorld::doAI() {
    pathingUpdater->update();
}

}  // namespace World
}  // namespace Project
