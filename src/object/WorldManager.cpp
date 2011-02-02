#include "WorldManager.h"
#include "config.h"

namespace Project {
namespace Object {

WorldManager::WorldManager() {
    world = new World();
    playerList = new PlayerList();
}

WorldManager::WorldManager(World *world, PlayerList *playerList)
    : world(world), playerList(playerList) {
    
}

WorldManager::~WorldManager() {
    delete world;
    delete playerList;
}

Player *WorldManager::getPlayer(int id) {
    return playerList->getPlayer(id);
}

void WorldManager::usePlayerList(PlayerList *playerList) {
    PlayerList::IteratorType iterator = this->playerList->getIterator();
    while(iterator.hasNext()) {
        Object::Player *player = iterator.next();
        world->removeObject(player);
    }
    // the players are deleted by the PlayerList's destructor
    delete this->playerList;
    
    // now add the new player list
    this->playerList = playerList;
    iterator = playerList->getIterator();
    while(iterator.hasNext()) {
        Object::Player *player = iterator.next();
        world->addObject(player);
    }
}

void WorldManager::initForClient(int id, const Math::Point &location) {
    Object::Player *player = new Object::Player(id, location);
    
    playerList->addPlayer(player);
    world->addObject(player);
}

}  // namespace Object
}  // namespace Project
