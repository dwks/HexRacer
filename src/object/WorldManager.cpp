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

Player *WorldManager::getPlayer(int id) {
    return playerList->getPlayer(id);
}

void WorldManager::usePlayerList(PlayerList *playerList) {
    PlayerList::IteratorType iterator = this->playerList->getIterator();
    while(iterator.hasNext()) {
        Object::Player *player = iterator.next();
        world->removeObject(player);
    }
    delete this->playerList;
    
    // now add the new player list
    this->playerList = playerList;
    iterator = playerList->getIterator();
    while(iterator.hasNext()) {
        Object::Player *player = iterator.next();
        world->addObject(player);
    }
}

void WorldManager::initForClient(int id) {
    Object::Player *player = new Object::Player(id, INITIAL_CAR_LOCATION);
    
    playerList->addPlayer(player);
    world->addObject(player);
}

}  // namespace Object
}  // namespace Project
