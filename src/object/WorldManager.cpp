#include "WorldManager.h"

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

}  // namespace Object
}  // namespace Project
