#include "WorldManager.h"
#include "config.h"

#include "settings/ProgramSettings.h"

#include "physics/Converter.h"
#include "map/PathTracker.h"
#include "bonus/GlobalBonusManager.h"
#include "settings/SettingsManager.h"

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
    
    LOG(WORLD, "Destructing WorldManager");
}

void WorldManager::clearPlayerList() {
    //playerList->clear();
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
    player->setPlayerName("The Lone Ranger");
    player->setTeamID(0);
    addPlayer(player);
	/*
	if (!Bonus::GlobalBonusManager::getInstance())
		new Bonus::GlobalBonusManager();
	*/
}

}  // namespace Object
}  // namespace Project
