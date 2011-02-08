#include "SDL.h"  // for SDL_GetTicks()

#include "GameWorld.h"

#include "settings/SettingsManager.h"
#include "settings/ProgramSettings.h"

#include "timing/AccelControl.h"

namespace Project {
namespace SDL {

void GameWorld::doConnect(const std::string &host, unsigned short port) {
    network = boost::shared_ptr<NetworkPortal>(new NetworkPortal());
    if(network->connectTo(
        GET_SETTING("network.host", "localhost").c_str(),
        GET_SETTING("network.port", 1820))) {
        
        Object::World *world;
        Object::PlayerList *playerList;
        
        network->waitForWorld(world, playerList);
        
        worldManager = boost::shared_ptr<Object::WorldManager>(
            new Object::WorldManager(world, playerList));
        clientData = boost::shared_ptr<ClientData>(
            new ClientData(network->getID()));
        playerManager = boost::shared_ptr<PlayerManager>(
            new PlayerManager(network->getID(), worldManager.get()));
        Settings::ProgramSettings::getInstance()->setConnected(true);
        isConnectedToNetwork = true;
    }
    else {
        worldManager = boost::shared_ptr<Object::WorldManager>(
            new Object::WorldManager());
        clientData = boost::shared_ptr<ClientData>(
            new ClientData());
        playerManager = boost::shared_ptr<PlayerManager>(
            new PlayerManager(0, worldManager.get()));
        isConnectedToNetwork = false;
    }
}

void GameWorld::construct(const std::string &host, unsigned short port) {
    physicsWorld = boost::shared_ptr<Physics::PhysicsWorld>(
        new Physics::PhysicsWorld());
    suspension = boost::shared_ptr<Physics::Suspension>(
        new Physics::Suspension(10));
    
    doConnect(host, port);
}

void GameWorld::construct2(Map::HRMap *map) {
    raceManager = boost::shared_ptr<Map::RaceManager>(
        new Map::RaceManager(map));

	pathManager = boost::shared_ptr<Map::PathManager>(
		new Map::PathManager(map->getPathNodes()));

	worldManager->setPathManager(pathManager.get());
    
    playerManager->setRaceManager(raceManager.get());
    
    if(!isConnectedToNetwork) {
        worldManager->initForClient(clientData->getPlayerID(),
            raceManager->startingPointForPlayer(clientData->getPlayerID()),
            raceManager->startingPlayerDirection());
    }
}

void GameWorld::checkNetwork() {
    network->checkNetwork();
}

void GameWorld::doPhysics() {
    suspension->setData(worldManager.get(), NULL);
    suspension->checkForWheelsOnGround();
    
    if(!Timing::AccelControl::getInstance()->getPaused()) {
        static Uint32 lastPhysicsTime = SDL_GetTicks();
        lastPhysicsTime += Timing::AccelControl::getInstance()
            ->getPauseSkip();
        Uint32 thisTime = SDL_GetTicks();
        
        physicsWorld->stepWorld((thisTime - lastPhysicsTime) * 1000);
        lastPhysicsTime = thisTime;
    }

}

void GameWorld::updatePlayerPathing() {

	Object::WorldManager::PlayerIteratorType iterator = worldManager->getPlayerIterator();
	while (iterator.hasNext()) {
		Object::Player* player = iterator.next();

		//Raycast downward to find the update point
		Math::Point origin_pos = player->getPosition();
		Math::Point dir_pos = origin_pos;
		dir_pos.setY(dir_pos.getY() - VEHICLE_PATH_RAY_MAX_HEIGHT);

		Math::Point update_pos;

		//Update if the player is above the track
		if (physicsWorld->raycastPoint(origin_pos, dir_pos, &update_pos)) {

			player->getPathTracker()->update(update_pos);

			//Start a new lap for the player if they have crossed the finish plane
			if (player->getPathTracker()->readyforNewLap() &&
				raceManager->getBoundingPlane().pointInside(origin_pos)) {
					player->getPathTracker()->startNewLap();
					LOG(WORLD, "Player: " << player->getID() << " has finished lap " << player->getPathTracker()->getNumLaps());

			}
			/*
			char prog_str [30];
			sprintf(prog_str, "%.4g", player->getPathTracker()->getLapProgress());
			LOG(WORLD, string("Lap Progress: ")+prog_str);
			*/
		}
	}

}

void GameWorld::render() {
    suspension->doStep(SDL_GetTicks());
    
    physicsWorld->render();
}

}  // namespace SDL
}  // namespace Project
