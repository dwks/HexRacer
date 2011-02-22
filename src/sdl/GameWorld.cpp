#include "SDL.h"  // for SDL_GetTicks()

#include "GameWorld.h"

#include "settings/SettingsManager.h"
#include "settings/ProgramSettings.h"

#include "event/EventSystem.h"
#include "event/PlayerAction.h"

#include "timing/AccelControl.h"

#include "history/PingTimeMeasurer.h"
#include "misc/Sleeper.h"

namespace Project {
namespace SDL {

bool GameWorld::doConnect(const std::string &host, unsigned short port) {
    network = boost::shared_ptr<NetworkPortal>(new NetworkPortal());
    
    // must happen before we try to connect, so that PingTimeMeasurer exists
    historian = boost::shared_ptr<History::Historian>(
        new History::Historian());
    
    if(network->connectTo(host.c_str(), port)) {
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
        LOG2(NETWORK, ERROR, "Could not connect to server "
            << host << ":" << port << "!");
        return false;
    }
    
    historian->setWorldManager(worldManager.get());
    historian->setPhysicsWorld(physicsWorld.get());
    
    return true;
}

void GameWorld::doNotConnect() {
    // do not initialize network, historian
    
    worldManager = boost::shared_ptr<Object::WorldManager>(
        new Object::WorldManager());
    clientData = boost::shared_ptr<ClientData>(
        new ClientData());
    playerManager = boost::shared_ptr<PlayerManager>(
        new PlayerManager(0, worldManager.get()));
    
    isConnectedToNetwork = false;
}

bool GameWorld::construct(const std::string &host, unsigned short port) {
    physicsWorld = boost::shared_ptr<Physics::PhysicsWorld>(
        new Physics::PhysicsWorld());
    suspension = boost::shared_ptr<Physics::Suspension>(
        new Physics::Suspension());
    
    if(host == "" && port == 0) {
        doNotConnect();
        return true;
    }
    else {
        return doConnect(host, port);
    }
}

void GameWorld::construct2(Map::HRMap *map) {
    raceManager = boost::shared_ptr<Map::RaceManager>(
        new Map::RaceManager(map));
	pathManager = boost::shared_ptr<Map::PathManager>(
		new Map::PathManager(map->getPathNodes()));
    pathingUpdater = boost::shared_ptr<Map::PathingUpdater>(
        new Map::PathingUpdater(worldManager.get(), raceManager.get()));
    
	worldManager->setPathManager(pathManager.get());
    
    playerManager->setRaceManager(raceManager.get());
    
    if(!isConnectedToNetwork) {
        worldManager->initForClient(clientData->getPlayerID(),
            raceManager->startingPointForPlayer(clientData->getPlayerID()),
            raceManager->startingPlayerDirection());
    }
}

void GameWorld::checkNetwork() {
    if(isConnectedToNetwork) {
        network->checkNetwork();
    }
}

void GameWorld::doPhysics() {
    suspension->setData(worldManager.get(), NULL);
    suspension->checkForWheelsOnGround();
    
    if(!Timing::AccelControl::getInstance()->getPaused()) {
        static Uint32 lastPhysicsTime = SDL_GetTicks();
        lastPhysicsTime += Timing::AccelControl::getInstance()
            ->getPauseSkip();
        Uint32 thisTime = SDL_GetTicks();
        
        /*LOG(PHYSICS,
            "step physics by " << (thisTime - lastPhysicsTime) * 1000);*/
        
        physicsWorld->stepWorld((thisTime - lastPhysicsTime));
        lastPhysicsTime = thisTime;
    }
}

void GameWorld::doAI() {
    pathingUpdater->update();
}

void GameWorld::render() {
    physicsWorld->render();
}

}  // namespace SDL
}  // namespace Project
