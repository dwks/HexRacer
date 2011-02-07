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

void GameWorld::render() {
    suspension->doStep(SDL_GetTicks());
    
    physicsWorld->render();
}

}  // namespace SDL
}  // namespace Project
