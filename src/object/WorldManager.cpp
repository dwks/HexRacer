#include "WorldManager.h"
#include "config.h"

#include "settings/ProgramSettings.h"

#include "physics/Converter.h"
#include "map/PathTracker.h"

namespace Project {
namespace Object {

WorldManager::WorldManager() {
    world = new World();
    playerList = new PlayerList(this);
}

WorldManager::WorldManager(World *world, PlayerList *playerList)
    : world(world), playerList(playerList) {
    
    playerList->setWorldManager(this);
}

WorldManager::~WorldManager() {
    delete world;
    delete playerList;
}

void WorldManager::addPlayer(Player *player) {
    if(world->getObject(player->getID())) return;
    
    world->addObject(player);
    playerList->addPlayer(player);

	if (pathManager)
		player->setPathTracker(new Map::PathTracker(*pathManager));
}

Player *WorldManager::getPlayer(int id) {
    return dynamic_cast<Player *>(world->getObject(id));
}

void WorldManager::initForClient(int id, const Math::Point &location, const Math::Point& direction) {
    Object::Player *player = new Object::Player(id, location, direction);
    addPlayer(player);
}

}  // namespace Object
}  // namespace Project
